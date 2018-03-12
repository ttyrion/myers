// diff.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

enum MoveType {
    MoveType_None,
    MoveType_Down,
    MoveType_Right,
    MoveType_Diagonal
};

struct Point {
    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }

    bool operator== (const Point& pt) const {
        return this->x == pt.x && this->y == pt.y;
    }

    bool operator!= (const Point& pt) const {
        return this->x != pt.x || this->y != pt.y;
    }
    
    MoveType operator >(const Point& pt2) {
        if (this->x == pt2.x && this->y + 1 == pt2.y) {
            return MoveType_Down;
        }
        else if (this->y == pt2.y && this->x + 1 == pt2.x) {
            return MoveType_Right;
        }
        else if (this->x + 1 == pt2.x && this->y + 1 == pt2.y) {
            return MoveType_Diagonal;
        }

        return MoveType_None;
    }

    int x = 0;
    int y = 0;
};

int main()
{
    std::string A = "ABACD";
    std::string B = "ABMMA";

    int length_of_A = A.length();
    int length_of_B = B.length();

    //snapshot of X for each d
    std::vector<std::map<int, int>> Xlist;

    //只存储x坐标,y可以由k=x-y算出
    std::map<int,int> X;
    //假设起始点(0,-1)
    X[1] = 0;
    bool go_ahead = true;
    //步数d
    //从0开始，因为d=0时，也可能沿对角线前进
    for (int d = 0; d <= length_of_A + length_of_B && go_ahead; d++) {
        //k-lines: k has odd or even values according to an odd or even d
        for (int k = -d; k <= d; k = k + 2) {
            //判断该从(d-1,k-1)向右走，还是从(d-1,k+1)向下走
            //当k!=d && k!=-d时, 可以从X[k + 1]向下走, 或者X[k - 1]向右走
            //具体怎么选择，判断x值：选择x大的,保证删除操作优于添加操作
            bool down = (k == -d || (k != d && X[k - 1] < X[k + 1]));
            int kPrev = down ? k + 1 : k - 1;
            //起始位置
            int xStart = X[kPrev];
            int yStart = xStart - kPrev;
            //中间位置
            int xTmp = down ? xStart : xStart + 1;
            int yTmp = xTmp - k;
            //此次move后的位置
            int xEnd = xTmp;
            int yEnd = yTmp;
            //判断能否总对角线(moving through a diagonal is free)
            while (xEnd < length_of_A && yEnd < length_of_B && A[xEnd] == B[yEnd]) {
                ++xEnd;
                ++yEnd;
            }
            X[k] = xEnd;
            
            //到终点,找到了最短路线
            if (xEnd >= length_of_A && yEnd >= length_of_B) {
                go_ahead = false;
                break;
            }
        }

        Xlist.push_back(X);
    }

    //回退，从终点到起点，找出路线
    std::vector<Point> path; //move的路线
    Point pt(A.length(), B.length());
    path.push_back(pt);
    for (int d = Xlist.size() - 1; pt.x > 0 || pt.y > 0; --d) {
        auto X = Xlist[d];
        int k = pt.x - pt.y;

        //结束位置
        int xEnd = X[k];
        int yEnd = pt.x - k;
        //结束位置前是否走的对角线
        while (xEnd > 0 && yEnd > 0 && xEnd <= length_of_A && yEnd <= length_of_B && A[xEnd-1] == B[yEnd-1]) {
            --xEnd;
            --yEnd;
            path.push_back(Point(xEnd, yEnd));
        }

        //down可以确定上一步的k线，对角线上的移动不影响d和k
        //并且,Xlist[d][k-1] == Xlist[d-1][k-1]; Xlist[d][k+1] == Xlist[d-1][k+1]
        bool down = (k == -d || (k != d && X[k - 1] < X[k + 1]));
        int kPrev = down ? k + 1 : k - 1;

        //起始位置
        int xStart = X[kPrev];
        int yStart = xStart - kPrev;

        ////中间位置
        //int xTmp = down ? xStart : xStart + 1;
        //int yTmp = xTmp - k;

        path.push_back(Point(xStart, yStart));

        pt.x = xStart;
        pt.y = yStart;
    }

    path.pop_back();//path第一个点是假设的起点(0,-1);

    //文字描述path代表的操作
    std::stringstream stream;
    if (path.size() >= 2) {        
        int size = path.size();
        for (int i = size - 2; i >= 0; --i) {
            Point prev_point = path[i+1];
            MoveType type = prev_point>(path[i]);
            switch (type) {
            case MoveType_Right: {
                stream << "-" << A[prev_point.x];

                break;
            }
            case MoveType_Down: {
                stream << "+" << B[prev_point.y];

                break;
            }
            case MoveType_Diagonal: {
                stream << "=" << A[prev_point.x];

                break;
            }
            }
        }
    }
    std::cout << "最短编辑路线如下：" << std::endl;
    std::cout << stream.str();
    std::string input;
    std::cin >> input;

    return 0;
}

