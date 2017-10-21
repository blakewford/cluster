#include <vector>

#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

struct point
{
    point()
    {
        x = 0;
        y = 0;
    }

    point(long X, long Y)
    {
        x = X;
        y = Y;
    }

    point(point& p)
    {
        x = p.x;
        y = p.y;
    }

    bool operator==(point p)
    {
        return x == p.x && y == p.y;
    }

    long x;
    long y;
};

typedef std::vector<point*> pointvector;

struct center
{
    center(pointvector* pv)
    {
        assigned = pv;
    }

    point computed;
    pointvector* assigned;
};

bool computeCenter(center& compute)
{
    long xAcc = 0, yAcc = 0;
    int index = compute.assigned->size();
    while(index--)
    {     
        xAcc += (*compute.assigned)[index]->x;
        yAcc += (*compute.assigned)[index]->y;
    }

    int divisor = compute.assigned->size();
    point p(xAcc/divisor, yAcc/divisor);
    bool test = p == compute.computed;
    compute.computed = p; 

    return test;
}

void assignPoints(pointvector points, std::vector<center*>& c)
{
    int count = points.size();
    while(count--)
    {
        point* p = points.at(count);
        int i = -1;
        int d = INT_MAX;
        long ci = c.size();
        while(ci--)
        {
            int tmp = sqrt(pow(c[ci]->computed.x - p->x, 2) + pow(c[ci]->computed.y - p->y, 2));
            if(tmp < d)
            {
                d = tmp;
                i = ci;
            }
        }
        c[i]->assigned->push_back(p); 
    }
}

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        printf("%s k <input file>\n", argv[0]);
        return -1;
    }

    const long k = strtol(argv[1], NULL, 10);

    FILE* input = fopen(argv[2], "r");

    size_t length = 0;
    char* buffer = NULL;
    pointvector All;
    std::vector<pointvector*> Groups;

    long count = k;
    while(count--)
    {
        Groups.push_back(new pointvector());
    }

    int iter = 0;
    while(getline(&buffer, &length, input) != -1)
    {
        if(!isspace(buffer[0]))
        {
            point* entry = new point;
            entry->x = strtol(buffer, NULL, 10);
            entry->y = strtol(strstr(buffer, "\t"), NULL, 10);
            All.push_back(entry);
            Groups[iter++]->push_back(entry);
            if(iter == k)
            {
                iter = 0;
            }
        }
    }

    std::vector<center*> Centers;

    count = k;
    while(count--)
    {
        Centers.push_back(new center(Groups[count]));
    }

    while(true)
    {
        //Compute centers
        count = k;
        bool stable = true;
        while(count--)
        {
            if(!computeCenter(*Centers[count]))
            {
                stable = false;
            }
        }

        if(stable)
        {
            break;
        }
        count = k;
        while(count--)
        {
            Groups[count]->clear();
        }
        //Assign points to closest center
        assignPoints(All, Centers);

        count = k;
        bool overconstrained = false;
        while(count--)
        {
            if(Groups[count]->size() == 0)
            {
                overconstrained = true;
            }
        }

        if(overconstrained)
            break;
    }

    count = k;
    while(count--)
    {
        while(!Groups[count]->empty())
        { 
            point* p = Groups[count]->back();
            printf("%ld %ld %ld\n", p->x, p->y, count+1); //Cluster output is 1 indexed
            delete p;  
            Groups[count]->pop_back();
        }
    }

    count = k;
    while(count--)
    {
        pointvector* pv = Groups.back();
        delete pv;
        Groups.pop_back();
    }

    count = k;
    while(count--)
    {
        center* c = Centers.back();
        delete c;
        Centers.pop_back();
    }

    printf("\n");
    fclose(input);

    return 0;
}
