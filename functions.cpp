#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

using namespace std;

Pixel** createImage(int width, int height) {
  cout << "Start createImage... " << endl;
  
  // Create a one dimensional array on the heap of pointers to Pixels 
  //    that has width elements (i.e. the number of columns)
  Pixel** image = new Pixel*[width];
  
  bool fail = false;
  
  for (int i=0; i < width; ++i) { // loop through each column
    // assign that column to a one dimensional array on the heap of Pixels
    //  that has height elements (i.e. the number of rows)
    image[i] = new Pixel[height];
    
    if (image[i] == nullptr) { // failed to allocate
      fail = true;
    }
  }
  
  if (fail) { // if any allocation fails, clean up and avoid memory leak
    // deallocate any arrays created in for loop
    for (int i=0; i < width; ++i) {
      delete [] image[i]; // deleting nullptr is not a problem
    }
    delete [] image; // delete array of pointers
    return nullptr;
  }
  
  // initialize cells
  //cout << "Initializing cells..." << endl;
  for (int row=0; row<height; ++row) {
    for (int col=0; col<width; ++col) {
      //cout << "(" << col << ", " << row << ")" << endl;
      image[col][row] = { 0, 0, 0 };
    }
  }
  cout << "End createImage... " << endl;
  return image;
}

void deleteImage(Pixel** image, int width) {
  cout << "Start deleteImage..." << endl;
  // avoid memory leak by deleting the array
  for (int i=0; i<width; ++i) {
    delete [] image[i]; // delete each individual array placed on the heap
  }
  delete [] image;
  image = nullptr;
}

// implement for part 1

int* createSeam(int length) {
  int* seamArray = new int[length];
  return seamArray;
}

void deleteSeam(int* seam) {
  delete [] seam;
}


bool loadImage(string filename, Pixel** image, int width, int height) {
  //opens PPM file and load the 2d image array with pixels using rgb values from the file
  //returns true if loading was a success, false otherwise
  ifstream pixels(filename);
  if(pixels.is_open()) {
    string type;
    int fileWidth;
    int fileHeight;
    int temp2;
    pixels >> type;
    if(type != "P3" && type != "p3") {
      cout << "Error: type is " << type << " instead of P3" << endl;
      return false;
    }
    string checker;
    pixels >> checker;
    for(long unsigned int i = 0; i < checker.length(); i++) {
          if(checker[i] != 45 && (checker[i] < 48 || checker[i] > 57)) {
            cout << "Error: read non-integer value" << endl;
            return false;
          }
    }
    fileWidth = stoi(checker);
    

    pixels >> checker;
    for(long unsigned int i = 0; i < checker.length(); i++) {
          if(checker[i] != 45 && (checker[i] < 48 || checker[i] > 57)) {
            cout << "Error: read non-integer value" << endl;
            return false;
          }
    }
    fileHeight = stoi(checker);

    if(fileWidth != width) {
      cout << "Error: input width (" << width << ") does not match value in file (" << fileWidth << ")" << endl;
      return false;
    }
    if(fileHeight != height) {
      cout << "Error: input height (" << height << ") does not match value in file (" << fileHeight << ")" << endl;
      return false;
    }
    pixels >> temp2;
    for(int row = 0; row < height; row++) {
      for(int col = 0; col < width; col++) {

        pixels >> checker;
        if(pixels.eof()) {
          cout << "Error: not enough color values" << endl;
          return false;
        }
        for(long unsigned int i = 0; i < checker.length(); i++) {
          if(checker[i] != 45 && (checker[i] < 48 || checker[i] > 57)) {
            cout << "Error: read non-integer value" << endl;
            return false;
          }
        }
        image[col][row].r = stoi(checker);

        pixels >> checker;
        if(pixels.eof()) {
          cout << "Error: not enough color values" << endl;
          return false;
        }
        for(long unsigned int i = 0; i < checker.length(); i++) {
          if(checker[i] != 45 && (checker[i] < 48 || checker[i] > 57)) {
            cout << "Error: read non-integer value" << endl;
            return false;
          }
        }
        image[col][row].g = stoi(checker);

        pixels >> checker;
        for(long unsigned int i = 0; i < checker.length(); i++) {
          if(checker[i] != 45 && (checker[i] < 48 || checker[i] > 57)) {
            cout << "Error: read non-integer value" << endl;
            return false;
          }
        }
        image[col][row].b = stoi(checker);

        if(image[col][row].r > 255 || image[col][row].r < 0) {
          cout << "Error: invalid color value " << image[col][row].r << endl;
          return false;
        }
        if(image[col][row].g > 255 || image[col][row].g < 0) {
          cout << "Error: invalid color value " << image[col][row].g << endl;
          return false;
        }
        if(image[col][row].b > 255 || image[col][row].b < 0) {
          cout << "Error: invalid color value " << image[col][row].b << endl;
          return false;
        }
        if(pixels.fail() && !pixels.eof()) {
          cout << "Error: read non-integer value" << endl;
          return false;
        }
        //cout << image[col][row].r << ", " <<  image[col][row].g << ", " << image[col][row].b << endl;
      }
    }
  } else {
    cout << "Error: failed to open input file - " << filename << endl;
    return false;
  }
  if(!pixels.eof()) {
    string temp;
    pixels >> temp;
    if(temp == "") {
      return true;
    }
    cout << "Error: too many color values" << endl;
    return false;
  }
  return true;
  
}

bool outputImage(string filename, const Pixel*const* image, int width, int height) {
  ofstream writePPM(filename);
  //we want to open the ppm file/create it
  //we then want to input P3 width and height and 225 (max color value)
  //after that we can iterate thru our 2d array of rgb values for each pixel
  //and write them to file row by row
  writePPM << "P3" << endl;
  writePPM << width << " " << height << endl;
  writePPM << 255 << endl;
  if(!writePPM.is_open()) {
    cout << "Error: failed to open output file - " << filename << endl;
    return false;
  }

  for(int row = 0; row < height; row++) {
    for(int col = 0; col < width; col++) {
      writePPM << image[col][row].r << endl;
      writePPM << image[col][row].g << endl;
      writePPM << image[col][row].b << endl;
    }
  }
  writePPM.close();
  return true;
}

int energy(const Pixel*const* image, int x, int y, int width, int height) { 
  //energy == absolute diff between left and right pixels R, G, and B values squared
  //and added together for x and y direction
  //for pixels on the boarder, we calculate energy by defining leftmost and right most cols as adj
  //same goes for top-bottom
  //we repeat this for x and y
  //2 cases, for both x and y
  //x, if x != 0 or width - 1 it is normal, else it is a border and we have to do special calc
  //y, if y != 0 or height - 1 it is normal, else it is a border and we have to do special calc

  //determine case and calculate x and y gradients

  //in the end add them and return

  //x
  int xGradient;
  int R, G, B;
  if(x != 0 && x != width - 1) {
    //normal
    R = abs(image[x - 1][y].r - image[x + 1][y].r);
    G = abs(image[x - 1][y].g - image[x + 1][y].g);
    B = abs(image[x - 1][y].b - image[x + 1][y].b);
  } else {
    //border
    if(width == 1) {
      R = 0;
      G = 0;
      B = 0;
    }
    else if(x == 0) {
      R = abs(image[width - 1][y].r - image[x + 1][y].r);
      G = abs(image[width - 1][y].g - image[x + 1][y].g);
      B = abs(image[width - 1][y].b - image[x + 1][y].b);
    } else {
      R = abs(image[x - 1][y].r - image[0][y].r);
      G = abs(image[x - 1][y].g - image[0][y].g);
      B = abs(image[x - 1][y].b - image[0][y].b);
    }
  }
  xGradient = (R * R) + (G * G) + (B * B);
  //y
  int yGradient;
  if(y != 0 && y != height - 1) {
    //normal
    R = abs(image[x][y - 1].r - image[x][y + 1].r);
    G = abs(image[x][y - 1].g - image[x][y + 1].g);
    B = abs(image[x][y - 1].b - image[x][y + 1].b);
  } else {
    //border
    if(height == 1) {
      R = 0;
      G = 0;
      B = 0;
    }
    else if(y == 0) {
      R = abs(image[x][y + 1].r - image[x][height - 1].r);
      G = abs(image[x][y + 1].g - image[x][height - 1].g);
      B = abs(image[x][y + 1].b - image[x][height - 1].b);
    } else {
      R = abs(image[x][y - 1].r - image[x][0].r);
      G = abs(image[x][y - 1].g - image[x][0].g);
      B = abs(image[x][y - 1].b - image[x][0].b);
    }
  }
  yGradient = (R * R) + (G * G) + (B * B);
  return xGradient + yGradient;
}

// implement for part 2

// uncomment for part 2
int loadVerticalSeam(const Pixel*const* image, int start_col, int width, int height, int* seam) {
  //given the starting col, we want to iterate thru every row
  //each iteration thru the row we can only use the pixels to the left, center, or right of the prev pixel
  //we want to determine the energy for each and use the smallest in our calc
  //each iteration we will append that min energy to our total energy and append the col# to our seam array
  //in the end we will return our total energy
  //first use a for loop which loops thru every row starting at row 2
  //we want to have our inital energy at start_col already in our calc along w it's index inside or seam array
  //we want to determine the energies of the pixesl adjacent to prev row's min
  //so we want to check seam[i - 1] + 1 and - 1 along with seam[i - 1]
  //once we find the min, append to our total energy and append col#

  int totalEnergy = energy(image, start_col, 0, width, height);
  seam[0] = start_col;
  for(int i = 1; i < height; i++) {
    int prevCol = seam[i - 1];
    int currMin = energy(image, prevCol, i, width, height);
    int currMinIndex = prevCol;
    //can do a seperate for loop which iterates thru preCol - 1 to prevCol + 1
    //and calcs energy at each col and tracks currMin and  currMinIndex
    //tie breaker, center, left, right
    for(int j = prevCol + 1; j >= prevCol - 1; j--) {
      if(j == prevCol || j < 0 || j > (width - 1))
        continue;
      int currEnergy = energy(image, j, i, width, height);
      if(currEnergy < currMin) {
        currMin = currEnergy;
        currMinIndex = j;
      }
    }
    totalEnergy += currMin;
    seam[i] = currMinIndex;
  }
  return totalEnergy;
}

int* findMinVerticalSeam(const Pixel*const* image, int width, int height) {
  //we want to determine the vertical seam for all cols
  //we then want to determine which one has the smallest energy
  //if there are more than 1 w same min energy, prioritize smaller col index
  //we need to keep track of both energy and seam array
  //utlize createSeam and deleteSeam functions
  //we need a pointer to keep track of our minSeam 
  //we also need a variable to keep track of our minEnergy so we can compare seams
  //we can have a for loop which iterates thru every col
  //in each col, we want to use createSeam() to make a new seam
  //we then want to use our loadVerticalSeam() func to populate this seam and return it's energy
  //if it's energy is less thann our current min, we want to switch it
  //and we also want to delete the memory of our old seam and set pointer to new seam
  //else we want to delete our current seam and continue to next interation
  //in the end we will return the min seam out of all of them

  int *minSeam = createSeam(height);
  int minEnergy = loadVerticalSeam(image, 0, width, height, minSeam);
  for(int i = 1; i < width; i++) {
    int *currSeam = createSeam(height);
    int currEnergy = loadVerticalSeam(image, i, width, height, currSeam);
    if(currEnergy < minEnergy) {
      minEnergy = currEnergy;
      deleteSeam(minSeam);
      minSeam = currSeam;
    } else {
      deleteSeam(currSeam);
    }

  }
  return minSeam;
}

void removeVerticalSeam(Pixel** image, int width, int height, int* verticalSeam) {
  for(int i = 0; i < height; i++) {
    for(int j = verticalSeam[i]; j < width - 1; j++) {
      image[j][i].r = image[j + 1][i].r;
      image[j][i].g = image[j + 1][i].g;
      image[j][i].b = image[j + 1][i].b;
    }
  }
}

 /*
int loadHorizontalSeam(const Pixel*const* image, int start_row, int width, int height, int* seam) {
  return 0;
}

int* findMinVerticalSeam(const Pixel*const* image, int width, int height) {
  return nullptr;
}

int* findMinHorizontalSeam(const Pixel*const* image, int width, int height) {
  return nullptr;
}

void removeVerticalSeam(Pixel** image, int width, int height, int* verticalSeam) {
}

void removeHorizontalSeam(Pixel** image, int width, int height, int* horizontalSeam) {
}
*/