#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdexcept>
#include <sstream>

// Constructor of TicTacToeTomek reads in a string containing the game state - assigns column score to decide winner:
// X = 1
// Y = -1
// so if a column, row or diagonal contain a score of +/- 4 then we have a winner.
// The 'T' token contains a value of 10, so we can also check for a winner (scores of 7 for Y and 13 for X)

// We also keep track of remaining spaces to check to see if the game is a draw or still in progress, in the
// event that there is no winner.

class TicTacToeTomek
{
public:
  TicTacToeTomek(std::string GameState);
  std::string getWinner();

  // next 4 functions are used to check score calculations by hand
  void getColumnScore(); 
  void getRowScore();
  void getLRdiagonalScore(){std::cout << LRdiagonalScore << std::endl;}
  void getRLdiagonalScore(){std::cout << RLdiagonalScore << std::endl;}

private:
  std::vector<int> columnScore;
  std::vector<int> rowScore;
  int LRdiagonalScore;
  int RLdiagonalScore;
  int remainingSpaces;
};

TicTacToeTomek::TicTacToeTomek(std::string GameState)
{

  for (int i = 0; i < 4; ++i)
    columnScore.push_back(0);

  remainingSpaces = 0;
  LRdiagonalScore = 0;
  RLdiagonalScore = 0;

  bool verbose = false; // turn this on to output the game grid (used to check the scores were being calculated correctly)
  int rowValue = 0;
  int columnTracker = 0;
  for (unsigned int i = 0; i < GameState.length(); ++i)
    {
      if (verbose) std::cout << GameState[i] << " ";
      if (GameState[i] == 'T'){
	rowValue += 10;
	columnScore[i % 4] += 10;
	if ((i % 4) == columnTracker) LRdiagonalScore += 10;
	if (i % 3 == 0 && i != 0 && i != 15) RLdiagonalScore += 10;
      }
      else if (GameState[i] == 'X'){
	rowValue += 1;
	columnScore[i % 4] += 1;
	if ((i % 4) == columnTracker) LRdiagonalScore += 1;
	if (i % 3 == 0 && i != 0 && i != 15)  RLdiagonalScore += 1;
      }
      else if (GameState[i] == 'O'){
	rowValue -= 1;
	columnScore[i % 4] -= 1;
	if ((i % 4) == columnTracker) LRdiagonalScore -= 1;
	if (i % 3 == 0 && i != 0 && i != 15) RLdiagonalScore -= 1;
      }
      else if (GameState[i] == '.'){
      	remainingSpaces++;
      }
 
      if ((i+1) % 4 == 0)
	{
	  columnTracker++;
	  rowScore.push_back(rowValue);
	  rowValue = 0;
	  if (verbose) std::cout << std::endl;
	}
    }
  if (verbose) std::cout << std::endl;

}

void TicTacToeTomek::getRowScore()
{
  for (std::vector<int>::const_iterator it = rowScore.begin(); it != rowScore.end(); ++it)
    std::cout << *it << " ";
  std::cout << std::endl;
}

void TicTacToeTomek::getColumnScore()
{
  for (std::vector<int>::const_iterator it = columnScore.begin(); it != columnScore.end(); ++it)
    std::cout << *it << " ";
  std::cout << std::endl;
}

std::string TicTacToeTomek::getWinner()
{

  if (LRdiagonalScore == 13 || LRdiagonalScore == 4) return "X won";
  if (LRdiagonalScore == 7 || LRdiagonalScore == -4) return "O won";
  if (RLdiagonalScore == 13 || RLdiagonalScore == 4) return "X won";
  if (RLdiagonalScore == 7 || RLdiagonalScore == -4) return "O won";

  for (int i = 0; i < rowScore.size(); ++i)
    {
      if (rowScore[i] == 13 || rowScore[i] == 4) return "X won";
      if (rowScore[i] == 7 || rowScore[i] == -4) return "O won";
      if (columnScore[i] == 13 || columnScore[i] == 4) return "X won";
      if (columnScore[i] == 7 || columnScore[i] == -4) return "O won";
    }

  if (remainingSpaces > 0) return "Game has not completed";
  else return "Draw";
}




int main()
{

  std::ifstream inputFile;
  inputFile.open("A-large-practice.in");

  std::string gameString;
  int lineCount = 0;

  getline(inputFile, gameString); // This is just used to throw away the integer in the first line of the input

  std::vector<std::string> gameList; // Stores the game strings that we'll evaluate in the TicTacToeTomek class
  std::string currentGame; 

  int lines = 0;

  // loop through input file, combine each 4 line game into a single line string (this in the input the class expects)
  while (getline(inputFile, gameString))
    {
      std::stringstream line(gameString); 
      std::string stringline;
      line >> stringline;
      if (stringline.empty()) continue;
      currentGame += stringline;
      lines++;
      if (lines == 4)
	{
	  gameList.push_back(currentGame);
	  currentGame = "";
	  lines = 0;
	}
    }

  // iterate through games: construct game, output result, destroy games
  for (int i = 0; i < gameList.size(); ++i)
    {
      TicTacToeTomek instance(gameList[i]);
      std::cout << "Case #" << i + 1 << ": " << instance.getWinner() << std::endl;
    }



  return 0;
}
