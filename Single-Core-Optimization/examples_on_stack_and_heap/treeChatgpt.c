#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_FEATURES 4
#define MAX_EXAMPLES 150

typedef struct {
  double sepal_length;
  double sepal_width;
  double petal_length;
  double petal_width;
  char label[20];
} iris_example;

typedef struct node {
  int feature;
  double threshold;
  struct node* left;
  struct node* right;
  char label[20];
} tree_node;

double gini_impurity(iris_example examples[MAX_EXAMPLES], int num_examples) {
  // Count the number of examples belonging to each class
  int class_counts[3] = {0, 0, 0};
  for (int i = 0; i < num_examples; i++) {
    if (strcmp(examples[i].label, "setosa") == 0) {
      class_counts[0]++;
    } else if (strcmp(examples[i].label, "versicolor") == 0) {
      class_counts[1]++;
    } else if (strcmp(examples[i].label, "virginica") == 0) {
      class_counts[2]++;
    }
  }

  // Calculate the Gini impurity
  double impurity = 1;
  for (int i = 0; i < 3; i++) {
    double probability = (double)class_counts[i] / num_examples;
    impurity -= probability * probability;
  }
  return impurity;
}

int find_best_split(iris_example examples[MAX_EXAMPLES], int num_examples, int* feature, double* threshold) {
  // Initialize the best Gini impurity to a large value
  double best_impurity = 1.0;

  // Iterate over all features and thresholds to find the best split
  for (int i = 0; i < MAX_FEATURES; i++) {
    for (int j = 0; j < num_examples; j++) {
      double current_threshold = examples[j].sepal_length;
      if (i == 1) current_threshold = examples[j].sepal_width;
      if (i == 2) current_threshold = examples[j].petal_length;
      if (i == 3) current_threshold = examples[j].petal_width;

      // Split the examples based on the current feature and threshold
      iris_example left[MAX_EXAMPLES], right[MAX_EXAMPLES];
      int num_left = 0, num_right = 0;
      for (int k = 0; k < num_examples; k++) {
        double value = examples[k].sepal_length;
        if (i == 1) value = examples[k].sepal_width;
        if (i == 2) value = examples[k].petal_length;
        if (i == 3) value = examples[k].petal_width;

        if (value < current_threshold) {
          left[num_left++] = examples[k];
        } else {
          right[num_right++] = examples[k];
        }
      }

      // Calculate the Gini impurity of the split
      double left_impurity = gini_impurity(left, num_left);
      double right_impurity = gini_impurity(right, num_right);
      double current_impurity = (double)num_left / num_examples * left_impurity +
                               (double)num_right / num_examples * right_impurity;

      // If the current split is better than the best split, update the best split
      if (current_impurity < best_impurity) {
        best_impurity = current_impurity;
        *feature = i;
        *threshold = current_threshold;
      }
    }
  }

  return best_impurity < 1.0;
}

tree_node* build_tree(iris_example examples[MAX_EXAMPLES], int num_examples) {
  tree_node* node = malloc(sizeof(tree_node));

  // If the examples are all the same class, create a leaf node
  int num_setosa = 0, num_versicolor = 0, num_virginica = 0;
  for (int i = 0; i < num_examples; i++) {
    if (strcmp(examples[i].label, "setosa") == 0) num_setosa++;
    if (strcmp(examples[i].label, "versicolor") == 0) num_versicolor++;
    if (strcmp(examples[i].label, "virginica") == 0) num_virginica++;
  }
  if (num_setosa == num_examples) {
    strcpy(node->label, "setosa");
    return node;
  }
  if (num_versicolor == num_examples) {
    strcpy(node->label, "versicolor");
    return node;
  }
  if (num_virginica == num_examples) {
    strcpy(node->label, "virginica");
    return node;
  }

  // Find the best split for the current examples
  int feature;
  double threshold;
  if (!find_best_split(examples, num_examples, &feature, &threshold)) {
    // If no split is found, create a leaf node with the most common class
    if (num_setosa > num_versicolor && num_setosa > num_virginica) {
      strcpy(node->label, "setosa");
    } else if (num_versicolor > num_virginica) {
      strcpy(node->label, "versicolor");
    } else {
      strcpy(node->label, "virginica");
    }
    return node;
  }

  // Split the examples and recursively build the left and right subtrees
  node->feature = feature;
  node->threshold = threshold;
  iris_example left[MAX_EXAMPLES], right[MAX_EXAMPLES];
  int num_left = 0, num_right = 0;
  for (int i = 0; i < num_examples; i++) {
    double value = examples[i].sepal_length;
    if (feature == 1) value = examples[i].sepal_width;
    if (feature == 2) value = examples[i].petal_length;
    if (feature == 3) value = examples[i].petal_width;

    if (value < threshold) {
      left[num_left++] = examples[i];
    } else {
      right[num_right++] = examples[i];
    }
  }
  node->left = build_tree(left, num_left);
  node->right = build_tree(right, num_right);
  return node;
}

void classify(tree_node* node, iris_example example) {
  if (strlen(node->label) > 0) {
    printf("Example classified as: %s\n", node->label);
    return;
  }

  double value = example.sepal_length;
  if (node->feature == 1) value = example.sepal_width;
  if (node->feature == 2) value = example.petal_length;
  if (node->feature == 3) value = example.petal_width;

  if (value < node->threshold) {
    classify(node->left, example);
  } else {
    classify(node->right, example);
  }
}

int main() {
  // Load the Iris dataset
  iris_example examples[MAX_EXAMPLES];
  FILE* fp = fopen("iris.csv", "r");
  char line[1024];
  int num_examples = 0;
  while (fgets(line, 1024, fp)) {
    sscanf(line, "%lf,%lf,%lf,%lf,%s", &examples[num_examples].sepal_length,
           &examples[num_examples].sepal_width, &examples[num_examples].petal_length,
           &examples[num_examples].petal_width, examples[num_examples].label);
    num_examples++;
  }
  fclose(fp);

  // Build the decision tree
  tree_node* root = build_tree(examples, num_examples);

  // Classify a new example
  iris_example example = {5.1, 3.5, 1.4, 0.2};
  classify(root, example);

  return 0;
}

