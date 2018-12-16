#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <random>
#include "environment.h"
#include "structs.h"
#include <fstream>

#define ROWS 12
#define COLS 12

using namespace std;

unsigned int visit = 0;
float learning_rate = 0.5;
float discount_factor = 0.9;
float epsilon = 0.01;
int max_iterations = 2000;
int steps = 0;
float negative_reward = -3;
int max_steps = 100;

std::vector<double> results;
std::vector<double> meanresult;

int start_x = 2;
int start_y = 2;

int goal_x = 11;
int goal_y = 11;

//std::vector<int> reward_x = {1, 3, 6,1, 4,  4, 3, 6, 4};
//std::vector<int> reward_y = {1, 1, 2, 3, 3, 4, 5, 5, 6};
std::vector<int> reward_mean= {81, 34, 55, 96, 17, 63, 86, 73, 68, 36};
std::vector<int> reward_lower = {38, 15, 9, 34, 10, 29, 8, 48, 21};

std::vector<int> reward_x = {4,7,9,1};
std::vector<int> reward_y = {4,8,9,1};


vector<double> eps = {0.01, 0.03, 0.1, 0.3, 1};

enum action {RIGHT, UP, LEFT, DOWN};

std::random_device rd;

vector<state> states;

cv::Mat img(ROWS, COLS, CV_8UC1, cv::Scalar(0));

// Get the next state given a current state s and an action a:
state getNextState(state s, action a)
{
    switch (a)
    {
    case RIGHT:
        return states[(s.x + 1) + s.y * img.cols];
    case UP:
        return states[s.x + (s.y - 1) * img.cols];
    case LEFT:
        return states[(s.x - 1) + s.y * img.cols];
    case DOWN:
        return states[s.x + (s.y + 1) * img.cols];
    }
    return s;
}

// Get the reward given a state and an action:
float GetReward(state s, action a)
{
    state next = getNextState(s, a);

    if (!(next.is_visited))
    {
        return next.reward;
    }
    else
        return negative_reward;
}

// Get the maximum reward given state s
float getMaxValue(state s)
{
    float current_max_value = -5000000;
        if (!s.is_outside_environment)
        {
            if (s.Q[visit].RIGHT > current_max_value)
                current_max_value = s.Q[visit].RIGHT;
            if (s.Q[visit].UP > current_max_value)
                current_max_value = s.Q[visit].UP;
            if (s.Q[visit].LEFT > current_max_value)
                current_max_value = s.Q[visit].LEFT;
            if (s.Q[visit].DOWN > current_max_value)
                current_max_value = s.Q[visit].DOWN;
        }
    return current_max_value;
    }

    // Get the best action computed according to the current state-value estimate:
action GetNextAction(state s)
{
    vector<action> actions = {RIGHT, UP, LEFT, DOWN};
    action res = actions[(rd() % 4)];
    state next_state = getNextState(s, res);
    if (rd() % 100 < 100*epsilon)
    {
        while (next_state.is_outside_environment)
        {
            res = actions[(rd() % 4)];
            next_state = getNextState(s, res);
        }
 //       std::cout << "RANDOM" << std::endl;
        return res;
    }
    else
    {
  //         std::cout << "NOT RANDOM" << std::endl;
        double best_action_value = -1000000;
        vector<action> best_action;
        if (s.Q[visit].RIGHT > best_action_value && !(getNextState(s, RIGHT).is_outside_environment))
        {
            best_action_value = s.Q[visit].RIGHT;
            best_action.push_back(RIGHT);
        }
        if (s.Q[visit].UP > best_action_value && !(getNextState(s, UP).is_outside_environment))
        {
            best_action.clear();
            best_action_value = s.Q[visit].UP;
            best_action.push_back(UP);
        }
        else if (s.Q[visit].UP == best_action_value && !(getNextState(s, UP).is_outside_environment))
        {
            best_action.push_back(UP);
        }
        if (s.Q[visit].LEFT > best_action_value && !(getNextState(s, LEFT).is_outside_environment))
        {
            best_action.clear();
            best_action_value = s.Q[visit].LEFT;
            best_action.push_back(LEFT);
        }
        else if (s.Q[visit].LEFT == best_action_value && !(getNextState(s, LEFT).is_outside_environment))
        {
            best_action.push_back(LEFT);
        }
        if (s.Q[visit].DOWN > best_action_value && !(getNextState(s, DOWN).is_outside_environment))
        {
            best_action.clear();
            best_action_value = s.Q[visit].DOWN;
            best_action.push_back(DOWN);
        }
        else if (s.Q[visit].DOWN == best_action_value && !(getNextState(s, DOWN).is_outside_environment))
        {
            best_action.push_back(DOWN);
        }

    return best_action[rd() % best_action.size()];
    }
}

void reset_map()
{
    visit = 0;
    state temp;
    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
        {
            temp = states[j + i * img.cols];
            temp.is_visited = 0;
            states[j + i * img.cols] = temp;
        }
}

void update_visit(state s)
{
 //   for (int i = 0; i < 9; i++)
    for (int i = 0; i < 9; i++)
        if (s.x == reward_x[i] && s.y == reward_y[i])
            visit |= 0b1 << i;
}


// Initialize state array
void initializeStateArray()
{
    states.clear();
    for (int i = 0; i < img.rows; i++)
    for (int j = 0; j < img.cols; j++)
    {
        state s;
        s.x = j;
        s.y = i;
        if (img.at<uchar>(i,j) == 1)
            s.is_outside_environment = 1;
        else
        {
            s.is_outside_environment = 0;
            s.reward = negative_reward;
        }
        for (int q = 0; q < 512; q++)
        {
            actionvalue a = {0, 0, 0, 0};
            s.Q.push_back(a);
        }
        s.is_visited = 0;
        states.push_back(s);
    }
}

void allocateRewards()
{

 //   for (int i = 0; i < 9; i++)
    for (int i = 0; i < 4; i++)
    {
        //int lower = reward_lower[i];
        //int lower = reward_mean[i] - 2;
        //int upper = 2 * reward_mean[i] - reward_lower[i];
        state s = states[reward_x[i] + reward_y[i] * img.cols];
        //s.reward = rd() % (upper - lower) + lower;
        //s.reward = rd() % upper;
        s.reward = 1000;
        states[s.x + s.y * img.cols] = s;
    }
}

int main()
{
    createEnvironment(img);


    double rewards[5][max_iterations];
    initializeStateArray();

    allocateRewards();

    for (int i  = 0; i < max_iterations; i++)
        meanresult.push_back(0);

    // Main learning loop
    for (int iteration = 0; iteration < max_iterations; iteration++)
    {
        state current_state = states[start_x + start_y * img.cols];


        /*
        std::cout << "Iteration: " << iteration << std::endl;
        std::cout << "(x,y) = (" << current_state.x << ", "<< current_state.y << ")" << std::endl;
        std::cout << " RIGHT: " << current_state.Q[visit].RIGHT << std::endl;
        std::cout << " UP: " << current_state.Q[visit].UP << std::endl;
        std::cout << " LEFT: " << current_state.Q[visit].LEFT << std::endl;
        std::cout << " DOWN: " << current_state.Q[visit].DOWN << std::endl;
        std::cout << "Goal reached: " << visit << std::endl << std::endl;
*/
        steps = 0;
        for (int e = 0; e < 5; e++)
        {


            epsilon = eps[e];
            double total_reward = 0;

              while (visit != 0b1111)//&& steps < 100)
            {

                action a         = GetNextAction(current_state);
                float reward     = GetReward(current_state, a);
                state next_state = getNextState(current_state, a);



                if (reward > 0)
                {
                    next_state.is_visited = 1;
                }
                total_reward += reward;


                update_visit(current_state);
/*
                std::cout << "Iteration: " << iteration << std::endl;
                std::cout << "(x,y) = (" << current_state.x << ", "<< current_state.y << ")" << std::endl;
                std::cout << " Reward: " << GetReward(current_state, a) << std::endl;
                std::cout << " RIGHT: " << current_state.Q[visit].RIGHT << std::endl;
                std::cout << " UP: " << current_state.Q[visit].UP << std::endl;
                std::cout << " LEFT: " << current_state.Q[visit].LEFT << std::endl;
                std::cout << " DOWN: " << current_state.Q[visit].DOWN << std::endl;
                std::cout << "Goal reached: " << visit << std::endl << std::endl;

*/

                if (a == RIGHT)
                {
                    current_state.Q[visit].RIGHT  += learning_rate * (reward + discount_factor * getMaxValue(next_state) - current_state.Q[visit].RIGHT);
                }
                if (a == UP)
                {
                    current_state.Q[visit].UP   += learning_rate * (reward + discount_factor * getMaxValue(next_state) - current_state.Q[visit].UP);
                }
                if (a == LEFT)
                {
                    current_state.Q[visit].LEFT += learning_rate * (reward + discount_factor * getMaxValue(next_state) - current_state.Q[visit].LEFT);
                }
                if (a == DOWN)
                {
                    current_state.Q[visit].DOWN += learning_rate * (reward + discount_factor * getMaxValue(next_state) - current_state.Q[visit].DOWN);
                }

                states[current_state.x + current_state.y * img.cols] = current_state;
                states[next_state.x + next_state.y * img.cols] = next_state;


                current_state = next_state;
                steps++;
            }
            results.push_back(total_reward);
           //std::cout << "epsilon: " << e << "   iteration: " << iteration << " reward: " << total_reward << " visited: " << visit << " epsilon " << epsilon << std::endl;
            std::cout << steps << std::endl;
            reset_map();
            current_state =  states[start_x + start_y * img.cols];
            //rewards[e][iteration] = total_reward;
            rewards[e][iteration] = steps;
            //rewards[0][iteration] = steps;
        }
        allocateRewards();
    }




    ofstream myfile;

    myfile.open ("/home/syrak/q_learning_2/results.txt");

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < max_iterations; j++)
        {
            myfile << rewards[i][j] << " ";
        }
        myfile << std::endl;
    }
    myfile.close();

    return 0;
}
