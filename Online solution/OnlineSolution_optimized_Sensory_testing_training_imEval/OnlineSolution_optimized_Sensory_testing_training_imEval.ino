#include <vector>
#include <math.h>
#include <string>
#include <SPI.h>
#include <SD.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>

#include <Arduino_LSM9DS1.h>

using namespace std;
struct Node
{
  std::vector<std::vector<float>> data;
  bool leaf = false;
  float split_val_node;
  short int id, parent_id, child_left_id, child_right_id, split_feature_node, size;
};


std::vector<std::vector<Node>> Forest;

/*
    Helper methods and variables
*/

short int node_counter;
unsigned char data_dim;
const int SAMPLE_SIZE = 256;
unsigned char NUMBER_OF_TREES = 1;
unsigned char TREE_DEPTH = (int)ceil(log2(SAMPLE_SIZE)); //Doesn't have to be based on sample size
unsigned short int node_ammount = 0;
const unsigned short int DATA_SET_SIZE = 500;

void setup()
{
  Serial.begin(9600);
  delay(10000);
  Serial.println("Initializing SD card...");
  Forest.reserve(25);
  while (!Serial);
  Serial.println("Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}


std::vector<std::vector<float>> cleanDataset(std::vector<std::vector<float>> & data)
{
  for (size_t i = 0; i < data.size(); i++)
  {
    for (size_t j = 0; j < data.size(); j++)
    {
      if (i != j)
      {
        if (data[i] == data[j])
        {
          data.erase(data.begin() + j);
        }
      }
    }
  }
  return data;
}

std::vector<std::vector<float>> random_unique(const std::vector<std::vector<float>> & data, int sample)
{
  std::vector<std::vector<float>> sub_sample;
  for (size_t i = 0; i < sample; i++)
  {
    int random = rand() % data.size();
    sub_sample.push_back(data[random]);
    for (size_t j = 0; j < sub_sample.size(); j++)
    {
      if (sub_sample[j] == data[random] && j != i)
      {
        sub_sample.pop_back();
        i--;
      }
    }
  }
  return sub_sample;
}

float c(float size)
{
  if (size > 2)
  {
    float temp = (2. * (log(size - 1.) + 0.5772156649)) - (2.*(size - 1.) / size);
    return temp;
  }
  if (size == 2)
  {
    return 1;
  }
  return 0;
}

int find_node_pos(const std::vector<Node> & tree, const Node & current_node, bool left)
{
  for (size_t i = 0; i < tree.size(); i++)
  {
    if (tree[i].id == current_node.child_left_id && left)
    {
      return i;
    }

    if (tree[i].id == current_node.child_right_id && !left)
    {
      return i;
    }
  }
}

float get_split(const std::vector<std::vector<float>> & data, int q_value)
{
  std::vector<float> x_values;

  for (size_t i = 0; i < data.size(); i++)
  {
    x_values.push_back(data[i][q_value]);
  }

  double min = *min_element(x_values.begin(), x_values.end());
  double max = *max_element(x_values.begin(), x_values.end());

  float split_value = (rand() / ((double)RAND_MAX + 1) * (max - min) + min);

  return split_value;
}

float path_length(const std::vector<vector<Node>> & forest, const std::vector<float> & data)
{
  std::vector<float> edges;
  float avg = 0;
  float avg_depth = 0;
  for (size_t j = 0; j < forest.size(); j++)
  {
    std::vector<Node> tree = forest[j];
    int current_node_pos = 0;
    char length = 0;

    while (length == 0 || !tree[current_node_pos].leaf)
    {
      float splitValue_attribute = data[tree[current_node_pos].split_feature_node];
      float splitValue_node = tree[current_node_pos].split_val_node;
      if (splitValue_attribute < splitValue_node)
      {
        current_node_pos = find_node_pos(tree, tree[current_node_pos], true);
        length += 1;
      }
      else
      {
        current_node_pos = find_node_pos(tree, tree[current_node_pos], false);
        length += 1;
      }
    }

    float leaf_size = tree[current_node_pos].size;
    float path_length = length + c(leaf_size);

    avg += path_length;
  }

  float average_path = avg / forest.size();
  return average_path;
}

float decision_function(const std::vector<vector<Node>> & forest, const std::vector<float> & dataPoint)
{
  float score = 0;
  float average_length = path_length(forest, dataPoint);
  if (DATA_SET_SIZE <= SAMPLE_SIZE)
  {
    score = 0.5 - pow(2, (-1 * average_length) / c(DATA_SET_SIZE));
  }
  else
  {
    score = 0.5 - pow(2, (-1 * average_length) / c(SAMPLE_SIZE));
  }
  return score;
}

void IsolationTree(std::vector<Node> & Tree, Node & j, char height, char height_limit)
{
  node_ammount++;
  j.size = j.data.size();

  if (j.data.size() > 1 && height <= height_limit)
  {
    j.split_feature_node = rand() % data_dim;

    float upper = j.data[0][j.split_feature_node];
    float lower = j.data[0][j.split_feature_node];

    while (upper == lower)
    {
      j.split_feature_node = rand() % data_dim;
      upper = j.data[0][j.split_feature_node];
      lower = j.data[0][j.split_feature_node];
      for (size_t i = 0; i < j.data.size(); i++)
      {
        if (j.data[i][j.split_feature_node] < lower)
        {
          lower = j.data[i][j.split_feature_node];
        }

        if (j.data[i][j.split_feature_node] > upper)
        {
          upper = j.data[i][j.split_feature_node];
        }
      }
    }
    j.split_val_node = get_split(j.data, j.split_feature_node);

    //Line 9
    Node left_child;
    node_counter++;
    left_child.id = node_counter;
    j.child_left_id = left_child.id;

    std::vector<std::vector<float>> temp;
    for (size_t i = 0; i < j.data.size(); i++)
    {
      if (j.data[i][j.split_feature_node] <= j.split_val_node)
      {
        left_child.data.emplace_back(j.data[i]);
      }
    }

    for (size_t i = 0; i < j.data.size(); i++)
    {
      if (j.data[i][j.split_feature_node] > j.split_val_node)
      {
        temp.emplace_back(j.data[i]);
      }
    }

    std::vector<std::vector<float>>().swap(j.data);
    Tree.emplace_back(j);
    left_child.parent_id = j.id;
    Tree[left_child.parent_id].child_left_id = left_child.id;

    IsolationTree(Tree, left_child, height + 1, height_limit);
    std::vector<std::vector<float>>().swap(left_child.data);

    //Line10
    Node right_child;
    node_counter++;
    right_child.data = temp;
    std::vector<std::vector<float>>().swap(temp);
    right_child.id = node_counter;
    j.child_right_id = right_child.id;
    right_child.parent_id = j.id;
    Tree[right_child.parent_id].child_right_id = right_child.id;

    IsolationTree(Tree, right_child, height + 1, height_limit);
    std::vector<std::vector<float>>().swap(right_child.data);

  }
  else
  {
    //Line 12
    j.leaf = true;
    j.id = node_counter;
    j.child_left_id = -1;
    j.child_right_id = -1;
    std::vector<std::vector<float>>().swap(j.data);
    Tree.emplace_back(j);
  }
}

//Algorithm 1
void IsolationForest(std::vector<Node> & tree, std::vector<std::vector<float>> & data)
{
  if (data.size() > SAMPLE_SIZE)
  {
    data = random_unique(data, SAMPLE_SIZE);
  }
  data = cleanDataset(data);
  Node root;
  node_counter = 0;
  root.id = node_counter;
  root.data = data;
  data_dim = data[0].size();
  IsolationTree(tree, root, 0, TREE_DEPTH);
}

void loop() {
  std::vector<std::vector<float>> sensoryData;
  float x, y, z;
  Serial.println("DATA GATHERING INITIATED!!!");
  unsigned short int dataGatherer = 0;
  Serial.println("GATHERING TRAINING DATA!!!");
  while (dataGatherer < DATA_SET_SIZE)
  {
    if (IMU.magneticFieldAvailable())
    {
      IMU.readMagneticField(x, y, z);
      //In case user want's to save the data set by simply copy and pasting the output
      /*Serial.print("data.push_back({");
      Serial.print(x);
      Serial.print(',');
      Serial.print(y);
      Serial.print(',');
      Serial.print(z);
      Serial.println("});");*/
      sensoryData.push_back({x, y, z});
      dataGatherer++;
    }
  }

  Serial.println("DATA GATHERED!!!");

  for (size_t i = 0; i < NUMBER_OF_TREES; i++)
  {
    randomSeed((i + 3) * 7);
    Forest.emplace_back();
    Serial.println(i);
    IsolationForest(Forest[i], sensoryData);
    Serial.print("Tree number  ");
    Serial.println(i);
  }

  Serial.println("ISOLATION FOREST CREATED!!");
  std::vector<std::vector<float>>().swap(sensoryData);

  dataGatherer = 0;
  Serial.println("GATHERING TEST DATA!!");
  //while (dataGatherer < DATA_SET_SIZE)
  while(1)
  {
    if (IMU.magneticFieldAvailable())
    {
      IMU.readMagneticField(x, y, z);
      std::vector<float> dataPoint = {x, y, z};
      float anomalyScores = decision_function(Forest, dataPoint);
      Serial.print("ANOMALY SCORE: ");
      Serial.println(anomalyScores);
      dataGatherer++;
    }
  }
  Serial.println("EVALUATING TEST DATA!!");

  while (1)
  {
  }
}
