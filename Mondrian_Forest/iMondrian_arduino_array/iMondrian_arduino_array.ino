#include <vector>
#include <math.h>
#include <string>
#include <SPI.h>
#include <SD.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>

using namespace std;

struct Node
{
  std::vector<std::vector<float>> data;
  float lower_val_dim[13];
  float upper_val_dim[13];
  bool leaf = false;
  float split_time, split_location;
  unsigned char id, parent_id, child_left_id, child_right_id, split_dimension, size;
};

std::vector<std::vector<Node>> Forest;
std::vector<Node> Tree;


/*
    Helper methods and variables
*/
short int node_counter;
unsigned char data_dim;
unsigned char NUMBER_OF_TREES = 10;
const float LIFE_TIME = 10;
const unsigned char NUMBER_OF_COLUMNS = 13;
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.0, 1.0);

std::default_random_engine generator;

std::vector<std::vector<float>> parsedCsv;
unsigned short int node_ammount = 0;
File myFile;

void setup()
{
  Serial.begin(9600);
  delay(10000);
  Serial.println("Initializing SD card...");
  parsedCsv.reserve(129);
  Forest.reserve(2);
  Tree.reserve(257);
  randomSeed(300);
  if (!SD.begin(10))
  {
  }

  myFile = SD.open("wine.csv", FILE_READ);
  while (myFile.available())
  {
    String list = myFile.readStringUntil('\n');

    char tab2[1024];
    strcpy(tab2, list.c_str());
    char * strtokIndx;
    strtokIndx = strtok(tab2, ",");
    int i = 1;
    std::vector<float> parsedRow;
    parsedRow.emplace_back(std::stof(strtokIndx));
    while (i < NUMBER_OF_COLUMNS)
    {
      strtokIndx = strtok(NULL, ",");
      parsedRow.emplace_back(std::stof(strtokIndx));
      i++;
    }
    parsedCsv.emplace_back(parsedRow);
  }
}


std::vector<std::vector<float>> cleanDataset(std::vector<std::vector<float>> data)
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

Node findChild(std::vector<Node> tree, int child_id)
{

  for (size_t i = 0; i < tree.size(); i++)
  {
    if (tree[i].id == child_id)
    {
      return tree[i];
    }
  }
}

float exp_dist(float lamda)
{
  std::exponential_distribution<double> distribution(lamda);
  int randLoops = rand() % 100;
  int i = 0;
  while (i < randLoops)
  {
    distribution(generator);
    i++;
  }
  return distribution(generator);
}

unsigned char get_dim(Node node, float lamda)
{
  float pick = dis(gen);


  float limit = 0;

  for (size_t i = 0; i < data_dim; i++)
  {
    limit += (node.upper_val_dim[i] - node.lower_val_dim[i]) / lamda;
    if (limit > pick)
    {
      return i;
    }
  }
}

/*
    Mondrian implementation
*/
//Algorithm 2
void SampleMondrianBlock(Node & j, float life_time, float t_parent)
{
  //Line 1
  //Line 2
  node_ammount++;
  Serial.println(node_ammount);
   Serial.println(data_dim);
  float lamda = 0;
  float min = 0;
  float max = 0;
  j.size = j.data.size();

  for (size_t i = 0; i < data_dim; i++)
  {
    for (size_t k = 0; k < j.data.size(); k++)
    {

      if (j.data[k][i] < min)
      {
        min = j.data[k][i];
      }

      if (j.data[k][i] > max)
      {
        max = j.data[k][i];
      }

    }
    lamda += (max - min);

    j.lower_val_dim[i] = min;
    j.upper_val_dim[i] = max;
  }
  //Line 3

  float E = exp_dist(lamda);

  //Line 4
  if (j.data.size() > 1)
  {
    //Line 5
    j.split_time = t_parent + E;

    //Line 6
    int sample_dimension = get_dim(j, lamda);
    //Line 7

    float upper = j.data[0][sample_dimension];
    float lower = j.data[0][sample_dimension];

    while (upper == lower)
    {
      sample_dimension = get_dim(j, lamda);
      upper = j.data[0][sample_dimension];
      lower = j.data[0][sample_dimension];
      for (size_t i = 0; i < j.data.size(); i++)
      {
        if (j.data[i][sample_dimension] < lower)
        {
          lower = j.data[i][sample_dimension];
        }

        if (j.data[i][sample_dimension] > upper)
        {
          upper = j.data[i][sample_dimension];
        }
      }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(lower, upper);
    float split_location = dis(gen);


    //Line 8


    //Save node attributes
    j.split_dimension = sample_dimension;
    j.split_location = split_location;

    Tree.emplace_back(j);

    //Line 9
    Node left_child;
    node_counter++;
    left_child.id = node_counter;
    j.child_left_id = left_child.id;

    std::vector<std::vector<float>> temp;
    for (size_t i = 0; i < j.data.size(); i++)
    {
      if (j.data[i][sample_dimension] <= split_location)
      {
        left_child.data.emplace_back(j.data[i]);
      }
    }

    for (size_t i = 0; i < j.data.size(); i++)
    {
      if (j.data[i][sample_dimension] > split_location)
      {
        temp.emplace_back(j.data[i]);
      }
    }

    std::vector<std::vector<float>>().swap(j.data);
    
    left_child.parent_id = j.id;
    Tree[left_child.parent_id].child_left_id = left_child.id;

    SampleMondrianBlock(left_child, life_time, j.split_time);
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
    SampleMondrianBlock(right_child, life_time, j.split_time);
    //std::vector<std::vector<float>>().swap(j.data);
    std::vector<std::vector<float>>().swap(right_child.data);

  }
  else
  {
    //Line 12
    j.split_time = life_time;
    j.leaf = true;
    j.child_left_id = -1;
    j.child_right_id = -1;
    j.id = node_counter;
    Tree.emplace_back(j);
  }
}

//Algorithm 1
void SampleMondrianTree(float life_time, std::vector<std::vector<float>> data)
{
  Node root;
  node_counter = 0;
  root.id = node_counter;
  root.data = data;
  root.split_time = 0;
  data_dim = data[0].size();
  SampleMondrianBlock(root, life_time, root.split_time);
}

//Algorithm 4
void ExtendMondrianBlock(float life_time, Node node, std::vector<float> instance, float t_parent)
{
  //Line 1
  float e_l = 0;
  float e_u = 0;

  for (size_t i = 0; i < instance.size(); i++)
  {
    float min = 0;
    float max = 0;
    if ((node.lower_val_dim[i] - instance[i]) > 0)
    {
      min = node.lower_val_dim[i] - instance[i];
    }

    if ((instance[i] - node.upper_val_dim[i]) > 0)
    {
      max  = instance[i] - node.upper_val_dim[i];
    }

    e_l += min;
    e_u += max;
  }

  //Line 2
  float lamda = e_l + e_u;
  float E = exp_dist(lamda);

  if (t_parent + E < node.split_time)
  {
    //Line 4
    int split_dimension = get_dim(node, lamda);
    //Line 5
    std::random_device rd;
    std::mt19937 gen(rd());
    float split_location;
    if (instance[node.split_dimension] > node.upper_val_dim[node.split_dimension])
    {
      std::uniform_real_distribution<> dis(node.upper_val_dim[node.split_dimension], instance[node.split_dimension]);
      split_location = dis(gen);
    }
    else
    {
      std::uniform_real_distribution<> dis(instance[node.split_dimension], node.lower_val_dim[node.split_dimension]);
      split_location = dis(gen);
    }

    //Line 6
    Node newParent;
    node_counter++;
    newParent.id = node_counter;

    newParent.parent_id = node.parent_id;
    Tree[node.id].parent_id = newParent.id;

    newParent.split_dimension = split_dimension;
    newParent.split_location = split_location;
    newParent.split_time = t_parent + E;

    Tree[node.child_left_id].parent_id = newParent.id;
    Tree[node.child_right_id].parent_id = newParent.id;

    //Fel, ska jämföras med instancen min(l_d, x), kan va löst med loopen
    //newParent.l_d = node.l_d;
    //newParent.u_d = node.u_d;
    for (size_t i = 0; i < instance.size(); i++)
    {
      if (node.lower_val_dim[i] < instance[i])
      {
        newParent.lower_val_dim[i] = node.lower_val_dim[i];
      }
      else
      {
        newParent.lower_val_dim[i] = instance[i];
      }

      if (node.upper_val_dim[i] > instance[i])
      {
        newParent.upper_val_dim[i] = node.upper_val_dim[i];
      }
      else
      {
        newParent.upper_val_dim[i] = instance[i];
      }
    }

    Node newLeaf;
    node_counter++;
    newLeaf.id = node_counter;
    newLeaf.leaf = true;
    newLeaf.parent_id = newParent.id;

    newLeaf.data.emplace_back(instance);

    if (instance[newParent.split_dimension] < newParent.split_location)
    {
      newParent.child_left_id = node.id;
      newParent.child_right_id = newLeaf.id;

      Tree[newParent.parent_id].child_left_id = newParent.id;

      Tree.emplace_back(newParent);
      SampleMondrianBlock(newLeaf, life_time, t_parent);
    }
    else
    {
      newParent.child_right_id = node.id;
      newParent.child_left_id = newLeaf.id;

      Tree[newParent.parent_id].child_right_id = newParent.id;

      Tree.emplace_back(newParent);
      SampleMondrianBlock(newLeaf, life_time, t_parent);
    }
  }
  else
  {
    //Line 11
    float min = *min_element(instance.begin(), instance.end());
    float max = *max_element(instance.begin(), instance.end());
    for (size_t i = 0; i < data_dim; i++)
    {
      if (node.lower_val_dim[i] > min)
      {
        node.lower_val_dim[i] = min;
      }

      if (node.upper_val_dim[i] < max)
      {
        node.upper_val_dim[i] = max;
      }
    }
    //Line 12
    if (node.leaf)
    {

    }
    else
    {
      //Line 13
      if (instance[node.split_dimension] <= node.split_location)
      {
        //Line 14
        ExtendMondrianBlock(life_time, findChild(Tree, node.child_left_id), instance, node.split_time);
      }
      else
      {
        //Line 14
        ExtendMondrianBlock(life_time, findChild(Tree, node.child_right_id), instance, node.split_time);
      }
    }
  }
}

//Algorithm 3
void ExtendMondrianTree(float life_time, std::vector<float> instance)
{
  ExtendMondrianBlock(life_time, Tree[0], instance, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  //parsedCsv = cleanDataset(parsedCsv);
  Node n;
  Serial.println(sizeof(n));

  for (size_t i = 0; i < NUMBER_OF_TREES; i++)
  {
    //Serial.println(i);
    SampleMondrianTree(LIFE_TIME, parsedCsv);
    Serial.println("asd");

    //Forest.emplace_back(Tree);
    Serial.println("asd1");
    std::vector<Node>().swap(Tree);
    Serial.println("asd2");
  }

  Serial.println("Tree Built");

  for (size_t j = 0; j < Forest.size(); j++)
  {
    Tree = Forest[j];
    for (size_t i = 0; i < Tree.size(); i++)
    {

      Serial.print("Node id: ");
      Serial.println(Tree[i].id);
      Serial.print(" Left child id: ");
      Serial.println(Tree[i].child_left_id);
      Serial.print(" Right child id: ");
      Serial.println(Tree[i].child_right_id);
      Serial.print(" Parent id: ");
      Serial.println(Tree[i].parent_id);
      Serial.print(" Leaf: ");
      Serial.println(Tree[i].leaf);
      Serial.print(" Data size ");
      Serial.println(Tree[i].size);
      Serial.print(" Split dim: ");
      Serial.println(Tree[i].split_dimension);
      Serial.print(" Split value: ");
      Serial.println(Tree[i].split_location);
      Serial.print(" Split time: ");
      Serial.println(Tree[i].split_time);
    }
  }

  /*for (size_t i = 0; i < Forest.size(); i++)
    {

    Tree = Forest[i];
    for (size_t j = 0; j < instances.size(); j++)
    {
      ExtendMondrianTree(LIFE_TIME, instances[j]);
    }
    Forest[i] = Tree;
    std::vector<Node>().swap(Tree);
    }
    Serial.println("");
    Serial.println("");
    Serial.println("");
    Serial.println("");
    Serial.println("");
    Serial.println("");
    Serial.println("Tree Extended");

    for (size_t j = 0; j < Forest.size(); j++)
    {
    Tree = Forest[j];
    for (size_t i = 0; i < Tree.size(); i++)
    {

      Serial.print("Node id: ");
      Serial.println(Tree[i].id);
      Serial.print(" Left child id: ");
      Serial.println(Tree[i].child_left_id);
      Serial.print(" Right child id: ");
      Serial.println(Tree[i].child_right_id);
      Serial.print(" Parent id: ");
      Serial.println(Tree[i].parent_id);
      Serial.print(" Leaf: ");
      Serial.println(Tree[i].leaf);
      Serial.print(" Data size ");
      Serial.println(Tree[i].size);
      Serial.print(" Split dim: ");
      Serial.println(Tree[i].split_dimension);
      Serial.print(" Split value: ");
      Serial.println(Tree[i].split_location);
      Serial.print(" Split time: ");
      Serial.println(Tree[i].split_time);
    }
    }

  */
  while (1)
  {
  }
}
