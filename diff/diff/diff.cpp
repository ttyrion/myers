// diff.cpp : �������̨Ӧ�ó������ڵ㡣
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

    //ֻ�洢x����,y������k=x-y���
    std::map<int,int> X;
    //������ʼ��(0,-1)
    X[1] = 0;
    bool go_ahead = true;
    //����d
    //��0��ʼ����Ϊd=0ʱ��Ҳ�����ضԽ���ǰ��
    for (int d = 0; d <= length_of_A + length_of_B && go_ahead; d++) {
        //k-lines: k has odd or even values according to an odd or even d
        for (int k = -d; k <= d; k = k + 2) {
            //�жϸô�(d-1,k-1)�����ߣ����Ǵ�(d-1,k+1)������
            //��k!=d && k!=-dʱ, ���Դ�X[k + 1]������, ����X[k - 1]������
            //������ôѡ���ж�xֵ��ѡ��x���,��֤ɾ������������Ӳ���
            bool down = (k == -d || (k != d && X[k - 1] < X[k + 1]));
            int kPrev = down ? k + 1 : k - 1;
            //��ʼλ��
            int xStart = X[kPrev];
            int yStart = xStart - kPrev;
            //�м�λ��
            int xTmp = down ? xStart : xStart + 1;
            int yTmp = xTmp - k;
            //�˴�move���λ��
            int xEnd = xTmp;
            int yEnd = yTmp;
            //�ж��ܷ��ܶԽ���(moving through a diagonal is free)
            while (xEnd < length_of_A && yEnd < length_of_B && A[xEnd] == B[yEnd]) {
                ++xEnd;
                ++yEnd;
            }
            X[k] = xEnd;
            
            //���յ�,�ҵ������·��
            if (xEnd >= length_of_A && yEnd >= length_of_B) {
                go_ahead = false;
                break;
            }
        }

        Xlist.push_back(X);
    }

    //���ˣ����յ㵽��㣬�ҳ�·��
    std::vector<Point> path; //move��·��
    Point pt(A.length(), B.length());
    path.push_back(pt);
    for (int d = Xlist.size() - 1; pt.x > 0 || pt.y > 0; --d) {
        auto X = Xlist[d];
        int k = pt.x - pt.y;

        //����λ��
        int xEnd = X[k];
        int yEnd = pt.x - k;
        //����λ��ǰ�Ƿ��ߵĶԽ���
        while (xEnd > 0 && yEnd > 0 && xEnd <= length_of_A && yEnd <= length_of_B && A[xEnd-1] == B[yEnd-1]) {
            --xEnd;
            --yEnd;
            path.push_back(Point(xEnd, yEnd));
        }

        //down����ȷ����һ����k�ߣ��Խ����ϵ��ƶ���Ӱ��d��k
        //����,Xlist[d][k-1] == Xlist[d-1][k-1]; Xlist[d][k+1] == Xlist[d-1][k+1]
        bool down = (k == -d || (k != d && X[k - 1] < X[k + 1]));
        int kPrev = down ? k + 1 : k - 1;

        //��ʼλ��
        int xStart = X[kPrev];
        int yStart = xStart - kPrev;

        ////�м�λ��
        //int xTmp = down ? xStart : xStart + 1;
        //int yTmp = xTmp - k;

        path.push_back(Point(xStart, yStart));

        pt.x = xStart;
        pt.y = yStart;
    }

    path.pop_back();//path��һ�����Ǽ�������(0,-1);

    //��������path����Ĳ���
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
    std::cout << "��̱༭·�����£�" << std::endl;
    std::cout << stream.str();
    std::string input;
    std::cin >> input;

    return 0;
}

