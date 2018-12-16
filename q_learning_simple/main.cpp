#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <random>
#include "environment.h"
#include "structs.h"
#include <fstream>

#define ROWS 20
#define COLS 20

using namespace std;

unsigned int visit = 0;
float learning_rate = 0.5;
float discount_factor = 0.9;
float epsilon = 0.0;
int max_iterations = 1000;
int steps = 0;

int max_steps = 100;

int goal_reward = 300;
float negative_reward = -1;

int start_x = 1;
int start_y = 1;

int goal_x = COLS - 2;
int goal_y = ROWS - 2;

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

    if (next.x == goal_x && next.y == goal_y)
        return goal_reward;
    else
        return negative_reward;
}

// Get the maximum reward given state s
float getMaxValue(state s)
{
    float current_max_value = -5000000;
        if (!s.is_outside_environment)
        {
            if (s.RIGHT > current_max_value)
                current_max_value = s.RIGHT;
            if (s.UP > current_max_value)
                current_max_value = s.UP;
            if (s.LEFT > current_max_value)
                current_max_value = s.LEFT;
            if (s.DOWN > current_max_value)
                current_max_value = s.DOWN;
        }
        if (current_max_value != -5000000)
            return current_max_value;
        else
            return 0;
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
        return res;
    }
    else
    {
        double best_action_value = -1000000;
        vector<action> best_action;
        if (s.RIGHT > best_action_value && !(getNextState(s, RIGHT).is_outside_environment))
        {
            best_action_value = s.RIGHT;
            best_action.push_back(RIGHT);
        }
        if (s.UP > best_action_value && !(getNextState(s, UP).is_outside_environment))
        {
            best_action.clear();
            best_action_value = s.UP;
            best_action.push_back(UP);
        }
        else if (s.UP == best_action_value && !(getNextState(s, UP).is_outside_environment))
        {
            best_action.push_back(UP);
        }
        if (s.LEFT > best_action_value && !(getNextState(s, LEFT).is_outside_environment))
        {
            best_action.clear();
            best_action_value = s.LEFT;
            best_action.push_back(LEFT);
        }
        else if (s.LEFT == best_action_value && !(getNextState(s, LEFT).is_outside_environment))
        {
            best_action.push_back(LEFT);
        }
        if (s.DOWN > best_action_value && !(getNextState(s, DOWN).is_outside_environment))
        {
            best_action.clear();
            best_action_value = s.DOWN;
            best_action.push_back(DOWN);
        }
        else if (s.DOWN == best_action_value && !(getNextState(s, DOWN).is_outside_environment))
        {
            best_action.push_back(DOWN);
        }

    return best_action[rd() % best_action.size()];
    }
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
        }
        states.push_back(s);
    }
}


vector<int> total_steps;

int main()
{
    createEnvironment(img);

    initializeStateArray();
    img.at<uchar>(1,1) = 2;
    img.at<uchar>(goal_y, goal_x) = 3;
    std::cout << img << std::endl;

    for (int iteration = 0; iteration < max_iterations; iteration++)
    {
        state current_state = states[start_x + start_y * img.cols];
        steps = 0;
        while(!(current_state.x == goal_x && current_state.y == goal_y))
        {

            action a         = GetNextAction(current_state);
            float reward     = GetReward(current_state, a);
            state next_state = getNextState(current_state, a);

            if (a == RIGHT)
                current_state.RIGHT  += learning_rate * (reward + discount_factor * getMaxValue(next_state) - current_state.RIGHT);
            if (a == UP)
                current_state.UP   += learning_rate * (reward + discount_factor * getMaxValue(next_state) - current_state.UP);
            if (a == LEFT)
                current_state.LEFT += learning_rate * (reward + discount_factor * getMaxValue(next_state) - current_state.LEFT);
            if (a == DOWN)
                current_state.DOWN += learning_rate * (reward + discount_factor * getMaxValue(next_state) - current_state.DOWN);

            states[current_state.x + current_state.y * img.cols] = current_state;

            current_state = next_state;
            steps++;
        }
        total_steps.push_back(steps);
    }

    ofstream myfile;
    myfile.open ("/home/syrak/q_learning/nemtest.txt");
    for (int j = 0; j < max_iterations; j++)
    {
        //std::cout << total_steps[j] << std::endl;
        myfile << j << " " << total_steps[j] << std::endl;
    }
    myfile.close();

    return 0;
}
