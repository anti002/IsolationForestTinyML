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
  float l_d[13];
  float u_d[13];
  bool leaf = false;
  float split_time, split_location;
  short int id, parent_id, child_left_id, child_right_id, split_dimension, size;
};

std::vector<std::vector<Node>> Forest;

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
  //Tree.reserve(300);
  randomSeed(300);
  if (!SD.begin(10))
  {
  }

  char rows_to_parsed = 0;
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
    //if (rows_to_parsed < 75)
    //{
      parsedCsv.emplace_back(parsedRow);
    //}
    rows_to_parsed++;
  }
}


std::vector<std::vector<float>> cleanDataset(std::vector<std::vector<float>>  data)
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

Node findChild(const std::vector<Node> & tree, int child_id)
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

unsigned char get_dim(const Node & node, float lamda)
{
  float pick = dis(gen);

  float limit = 0;
  
  for (size_t i = 0; i < data_dim; i++)
  {
    limit += (node.u_d[i] - node.l_d[i]) / lamda;
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
void SampleMondrianBlock(std::vector<Node> & Tree, Node & j, float life_time, float t_parent)
{
  //Line 1
  //Line 2
  node_ammount++;
  Serial.println(node_ammount);
  float lamda = 0;
  float min = 0;
  float max = 0;
  j.size = j.data.size();


  //j.u_d.reserve(j.data[0].size());
  //j.l_d.reserve(j.data[0].size());

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
    //j.l_d.emplace_back(min);
    //j.u_d.emplace_back(max);
    j.l_d[i] = min;
     j.u_d[i] = max;
  }
  //Line 3

  float E = exp_dist(lamda);

  //Line 4
  if (j.data.size() > 1)
  {
    //Line 5
    j.split_time = t_parent + E;

    //Line 6
    unsigned char sample_dimension = get_dim(j, lamda);
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
    //std::random_device rd;
    //std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(lower, upper);
    float split_location = dis(gen);

    //Line 8, changed into line 9 for saving memory purposes


    //Save node features
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

    SampleMondrianBlock(Tree, left_child, life_time, j.split_time);
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
    SampleMondrianBlock(Tree, right_child, life_time, j.split_time);
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
void SampleMondrianTree(std::vector<Node> & tree, float life_time,  std::vector<std::vector<float>>  data)
{
  Node root;
  node_counter = 0;
  root.id = node_counter;
  root.data = data;
  root.split_time = 0;
  data_dim = data[0].size();
  SampleMondrianBlock(tree, root, life_time, root.split_time);
}

//Algorithm 4
void ExtendMondrianBlock(std::vector<Node> & Tree, float life_time, Node node, 
                         const std::vector<float> & instance, float t_parent)
{
  //Line 1
  float e_l = 0;
  float e_u = 0;

  for (size_t i = 0; i < instance.size(); i++)
  {
    float min = 0;
    float max = 0;
    if ((node.l_d[i] - instance[i]) > 0)
    {
      min = node.l_d[i] - instance[i];
    }

    if ((instance[i] - node.u_d[i]) > 0)
    {
      max  = instance[i] - node.u_d[i];
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
    unsigned char split_dimension = get_dim(node, lamda);
    //Line 5
    //std::random_device rd;
    //std::mt19937 gen(rd());
    float split_location;
    if (instance[node.split_dimension] > node.u_d[node.split_dimension])
    {
      std::uniform_real_distribution<> dis(node.u_d[node.split_dimension], instance[node.split_dimension]);
      split_location = dis(gen);
    }
    else
    {
      std::uniform_real_distribution<> dis(instance[node.split_dimension], node.l_d[node.split_dimension]);
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
      if (node.l_d[i] < instance[i])
      {
       // newParent.l_d.emplace_back(node.l_d[i]);
      }
      else
      {
      //  newParent.l_d.emplace_back(instance[i]);
      }

      if (node.u_d[i] > instance[i])
      {
       // newParent.u_d.emplace_back(node.u_d[i]);
      }
      else
      {
      //  newParent.u_d.emplace_back(instance[i]);
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
      SampleMondrianBlock(Tree, newLeaf, life_time, t_parent);
    }
    else
    {
      newParent.child_right_id = node.id;
      newParent.child_left_id = newLeaf.id;

      Tree[newParent.parent_id].child_right_id = newParent.id;

      Tree.emplace_back(newParent);
      SampleMondrianBlock(Tree, newLeaf, life_time, t_parent);
    }
  }
  else
  {
    //Line 11
    float min = *min_element(instance.begin(), instance.end());
    float max = *max_element(instance.begin(), instance.end());
    for (size_t i = 0; i < data_dim; i++)
    {
      if (node.l_d[i] > min)
      {
        node.l_d[i] = min;
      }

      if (node.u_d[i] < max)
      {
        node.u_d[i] = max;
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
        ExtendMondrianBlock(Tree, life_time, findChild(Tree, node.child_left_id), instance, node.split_time);
      }
      else
      {
        //Line 14
        ExtendMondrianBlock(Tree, life_time, findChild(Tree, node.child_right_id), instance, node.split_time);
      }
    }
  }
}

//Algorithm 3
void ExtendMondrianTree(std::vector<Node> &Tree, float life_time, const std::vector<float> & instance)
{
  ExtendMondrianBlock(Tree, life_time, Tree[0], instance, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  //parsedCsv = cleanDataset(parsedCsv);

  for (size_t i = 0; i < NUMBER_OF_TREES; i++)
  {
    Forest.emplace_back();
    SampleMondrianTree(Forest[i], LIFE_TIME, parsedCsv);
    Serial.print("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@:  ");
    Serial.println(i);

  }

  Serial.println("Tree Built");

  for (size_t j = 0; j < Forest.size(); j++)
  {
    std::vector<Node> & Tree = Forest[j];
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
