#include "main.h"
/****************************************************************
 * Main program for Homework 2.
 *
 * Author/copyright:  Duncan Buell. All rights reserved.
 * Used with permission and modified by: Prashant Thirumal
 * Date: 10 September 2018
 *
 * This program will read 10 pairs of 'double' numbers (hard
 * coded as 10) from standard input into a 'vector'.
 * The pairs are taken as (x, y) coordinates in the plane.
 * The program will compute the minimum distance between any
 * two pairs and output the minimum distance as well as the
 * subscript numbers that produce the minimum.
 *
 * We do this by brute force.  We run a double loop and compute
 * the distance, saving the subscripts of the pairs with the
 * current min distance.
 **/

// method to calculate Euclidean Distance
double euclidean(pair<double, double> pair1,
                 pair<double, double> pair2) {
  double distance = sqrt(pow((pair2.first - pair1.first), 2) +
                         pow((pair2.second - pair1.second), 2));
  return distance;
}

int main(int argc, char *argv[]) {
  vector<pair<double, double> > vector_values;
  double x;
  double y;
  int kNumberOfRuns = 10;  //  hard coding for loop to run 10 times
  pair<double, double> min_pair1;
  pair<double, double> min_pair2;
  pair<double, double> min_pair3;
  int pair_label1 = 0;
  int pair_label2 = 0;
  double min = 0.0;

  // fill in the vector with the values
  for (int i = 0; i < kNumberOfRuns; ++i) {
    pair <double, double> a_pair;
    cin >> x >> y;
    a_pair.first = x;
    a_pair.second = y;
    vector_values.push_back(a_pair);
  }

  // print out the values in the vector

  for (int i = 0; i < kNumberOfRuns; ++i) {
    std::cout << i << " " <<  vector_values[i].first << " , "
                   << vector_values[i].second << std::endl;
  }

  // Double for loop to calculate distance and store the pairs

  for (int i = 0; i < vector_values.size(); ++i) {
    min_pair1 = vector_values[i];
    for (int j = 0; j < vector_values.size(); ++j) {
      if (j == i) {  // skip the current pair if it is the same as the previous
        continue;
      }

      double d = euclidean(min_pair1, vector_values[j]);

        // store the pairs that resulted in the minimum distance

        if (min == 0 || d < min) {
        min = d;
        min_pair3 = min_pair1;
        min_pair2 = vector_values[j];
        pair_label1 = i;
        pair_label2 = j;
        std::cout << "NEW MIN AT " << pair_label1 << " " << pair_label2
                  << " Value: " << d << std::endl;
      }
    }
  }

  std::cout << "Pairs that resulted in minimum distance" << std::endl;
  std::cout << pair_label1 << "  " << min_pair3.first << " , "
            << min_pair3.second << std::endl;
  std::cout << pair_label2 << "  " << min_pair2.first << " , "
            << min_pair2.second << std::endl;


  return 0;
}
