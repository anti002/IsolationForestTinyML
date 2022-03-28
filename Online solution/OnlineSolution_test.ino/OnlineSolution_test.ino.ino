#include <vector>
#include <math.h>
#include <string>
#include <SPI.h>
#include <SD.h>
#include <fstream>
#include <sstream>

using namespace std;

struct DecisionNode
{
  unsigned char child_left;
  unsigned char child_right;
  std::vector<std::vector<float>> data_left;
  std::vector<std::vector<float>> data_right;
  unsigned char q_value;
  float x_value;
};

struct Leaf
{
  int n_samples;
  unsigned char depth;
};

struct Node
{
  Leaf leaf;
  DecisionNode decision_node;
  bool isLeaf;
  unsigned char node_id;
};

std::vector<Node> node_stack;
unsigned char current_node_id = 0;

std::vector<std::vector<float>> parsedCsv;
File myFile;
const unsigned char NUMBER_OF_COLUMNS = 13;
const int NUMBER_OF_TREES = 100;
const int SAMPLE_SIZE = 256;

void setup()
{
  Serial.begin(9600);
  delay(5000);
  Serial.println("Initializing SD card...");

  if (!SD.begin(10))
  {
  }

  myFile = SD.open("test.csv", FILE_READ);
  while (myFile.available())
  {
    String list = myFile.readStringUntil('\n');

    char tab2[1024];
    strcpy(tab2, list.c_str());
    char * strtokIndx;
    strtokIndx = strtok(tab2, ",");
    int i = 1;
    std::vector<float> parsedRow;
    parsedRow.push_back(std::stof(strtokIndx));
    while (i < NUMBER_OF_COLUMNS)
    {
      strtokIndx = strtok(NULL, ",");
      parsedRow.push_back(std::stof(strtokIndx));
      i++;
    }
    parsedCsv.push_back(parsedRow);
  }
}

float c(float size)
{
  if (size > 2)
  {
    float temp = (2 * (log(size - 1) + 0.5772156649)) - (2 * (size - 1) / size);
    return temp;
  }
  if (size == 2)
  {
    return 1;
  }
  return 0;
}

template<class BidiIter >
BidiIter random_unique(BidiIter begin, BidiIter end, size_t num_random)
{
  size_t left = std::distance(begin, end);
  while (num_random--)
  {
    BidiIter r = begin;
    std::advance(r, rand() % left);
    std::swap(*begin, *r);
    ++begin;
    --left;
  }
  return begin;
}

std::vector<float> path_length(std::vector<Node> tree, std::vector<std::vector<float>> parsedCsv)
{
  std::vector<float> edges;

  for (size_t i = 0; i < parsedCsv.size(); i++)
  {
    float avg = 0;
    int temp_id = tree.size() - 1;
    while (!tree[temp_id].isLeaf)
    {
      int q_value = tree[temp_id].decision_node.q_value;
      float splitValue_data_set = parsedCsv[i][q_value];
      float splitValue_node = tree[temp_id].decision_node.x_value;

      if (splitValue_data_set < splitValue_node)
      {
        int key = tree[temp_id].decision_node.child_left;
        for (size_t k = 0; k < tree.size(); k++)
        {
          if (tree[k].node_id == key)
          {
            temp_id = k;
            break;
          }
        }
      }
      else
      {
        int key = tree[temp_id].decision_node.child_right;
        for (size_t l = 0; l < tree.size(); l++)
        {
          if (tree[l].node_id == key)
          {
            temp_id = l;
            break;
          }
        }
      }
    }
    avg += tree[temp_id].leaf.depth + c(tree[temp_id].leaf.n_samples);
    edges.push_back(avg);
  }
  return edges;
}

std::vector<float> decision_function(float average_length[], std::vector<std::vector<float>> parsedCsv)
{
  std::vector<float> scores;
  for (size_t i = 0; i < 129; i++)
  {
    scores.push_back(0.5 - pow(2, (-1 * average_length[i]) / c(parsedCsv.size())));
  }
  return scores;
}

class iTree
{
  public:
    unsigned char current_height, height_limit;
    std::vector<std::vector<float>> data_left;
    std::vector<std::vector<float>> data_right;

    iTree (int current_height, int height_limit)
    {
      this->current_height = current_height;
      this->height_limit = height_limit;
    }

    Node fit(std::vector<std::vector<float>> sub_sample)
    {
      Node current_node;
      if (current_height >= height_limit || sub_sample.size() <= 1)
      {
        current_node.isLeaf = true;
        Leaf leaf;
        current_node.node_id = current_node_id;
        current_node_id++;
        leaf.n_samples = sub_sample.size();
        leaf.depth = this->current_height;
        current_node.leaf = leaf;
        node_stack.push_back(current_node);
        return current_node;
      }
      else
      {
        current_node.isLeaf = false;
        DecisionNode decision_node;
        decision_node.q_value = rand() % sub_sample[0].size();
        decision_node.x_value = sub_sample[rand() % sub_sample.size()][decision_node.q_value];
        current_node.decision_node = decision_node;
        current_node.node_id = current_node_id;
        current_node_id++;

        //split the data
        for (size_t i = 0; i < sub_sample.size(); i++)
        {
          if (sub_sample[i][decision_node.q_value] < decision_node.x_value)
          {
            this->data_left.push_back(sub_sample[i]);
          }
          else
          {
            this->data_right.push_back(sub_sample[i]);
          }
        }

        iTree left_subtree = iTree(this->current_height + 1, this->height_limit);
        iTree right_subtree = iTree(this->current_height + 1, this->height_limit);

        Node left_node = left_subtree.fit(data_left);
        Node right_node = right_subtree.fit(data_right);

        current_node.decision_node.child_left = left_node.node_id;
        current_node.decision_node.child_right = right_node.node_id;

        node_stack.push_back(current_node);
        return current_node;
      }
    };
};

class iForest
{
  public:
    int t_trees, sample_size;
    std::vector<float> anomaly_scores;
    iForest (int t_trees, int sample_size)
    {
      this->t_trees = t_trees;
      this->sample_size = sample_size;
    }

    void fit(std::vector<std::vector<float>> dataSet)
    {
      std::vector<std::vector<float>> dataSet_ = dataSet;
      float path_lengths [dataSet.size()] = {0};
      std::vector<float> average_paths;
      int height_limit = (int)ceil(log2(this->sample_size));

      if (dataSet.size() < sample_size)
      {
        this->sample_size = dataSet.size();
      }
      for (size_t i = 0; i < t_trees; i++)
      {
        random_unique(dataSet.begin(), dataSet.end(), this->sample_size);

        iTree tree = iTree(0, height_limit);
        Node root = tree.fit(dataSet);
        std::vector<float> lengths = path_length(node_stack, dataSet_);

        for (size_t j = 0; j < lengths.size(); j++)
        {
          path_lengths[j] = path_lengths[j] + lengths[j];
        }
        current_node_id = 0;
        std::vector<Node>().swap(node_stack);
      }

      for (size_t j = 0; j < dataSet_.size(); j++)
      {
        path_lengths[j] = path_lengths[j] / NUMBER_OF_TREES;
      }
      anomaly_scores = decision_function(path_lengths, dataSet_);
    };
};

void loop()
{
  iForest clf = iForest(NUMBER_OF_TREES, SAMPLE_SIZE);

  clf.fit(parsedCsv);
  for (size_t i = 0; i < clf.anomaly_scores.size(); i++)
  {
    Serial.println(clf.anomaly_scores[i], 8);
  }
  while (true)
  {
  }
}
