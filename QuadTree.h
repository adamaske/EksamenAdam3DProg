#ifndef QUADTREE_H
#define QUADTREE_H
#include <iostream>
#include <vector>
#include <QDebug>
typedef std::pair<double, double> Point2D;
template<class ID, class T>
class QuadTree{
public:
    QuadTree(const Point2D& v1, const Point2D& v2, const Point2D& v3, const Point2D& v4);

    void subDivide(int n);

    QuadTree* insert(const Point2D& point, const ID& id, const T& object);

    QuadTree* find(const Point2D& p);

    std::vector<std::pair<ID, T>> mObjects;

    typename std::vector<T>::iterator begin()
    {
        TraverseAll(m_allObjects);
        return m_allObjects.begin();
    }
    typename std::vector<T>::iterator end()
    {
        return m_allObjects.end();
    }
    bool isLeaf();
private:

    std::vector<T> m_allObjects;

            void TraverseAll(std::vector<T>& allObjects);
    QuadTree* m_ne;
    QuadTree* m_nw;
    QuadTree* m_se;
    QuadTree* m_sw;
    Point2D v_ne;
    Point2D v_nw;
    Point2D v_se;
    Point2D v_sw;
};

template<class ID, class T>
bool QuadTree<ID, T>::isLeaf()
{
    if(m_ne == nullptr && m_nw == nullptr && m_se == nullptr && m_sw == nullptr){
        return true;
    }else{
        return false;
    }
}

template<class ID, class T>
QuadTree<ID, T>::QuadTree(const Point2D& v1, const Point2D& v2, const Point2D& v3, const Point2D& v4)
    : m_se{ nullptr }, m_sw{ nullptr }, m_ne{ nullptr }, m_nw{ nullptr }
{
    mObjects.clear();
    v_ne = v1;
    v_nw = v2;
    v_se = v3;
    v_sw = v4;

}
template<class ID, class T>
void QuadTree<ID, T>::subDivide(int n)
{
    if (n > 0)
    {
        //Halway between southeast and southwest
        Point2D v1 = Point2D{ (v_sw.first + v_se.first) / 2, (v_sw.second + v_se.second) / 2 };
        //South east and north east
        Point2D v2 = Point2D{ (v_ne.first + v_se.first) / 2, (v_ne.second + v_se.second) / 2 };
        //Norh east and north west
        Point2D v3 = Point2D{ (v_ne.first + v_nw.first) / 2, (v_ne.second + v_nw.second) / 2 };
        //Norh west and southwest
        Point2D v4 = Point2D{ (v_sw.first + v_nw.first) / 2, (v_sw.second + v_nw.second) / 2 };
        //Middle
        Point2D m = Point2D { (v_ne.first + v_sw.first) / 2, (v_ne.second + v_sw.second) / 2 };
        m_ne = new QuadTree(v_ne, v3, v2, m);
        m_ne->subDivide(n - 1);
        m_nw = new QuadTree(v3, v_nw, m, v4);
        m_nw->subDivide(n - 1);
        m_se = new QuadTree(v1, m, v_se, v4);
        m_se->subDivide(n - 1);
        m_sw = new QuadTree(m, v4, v1, v_sw);
        m_sw->subDivide(n - 1);
    }
}
template<class ID, class T>
QuadTree<ID, T>* QuadTree<ID, T>::insert(const Point2D& point, const ID& id, const T& object)
{
    //Only put objects in leafs
    if (isLeaf())
    {
        //If this is a leaf, add it to this and return
        mObjects.push_back(std::pair<ID, T>{id, object});
        return this;
    }
    else
    {
        //If this is not a leaf, find the middle and determin what quad it should go in
        Point2D m = Point2D{ (v_ne.first + v_se.first) / 2, (v_nw.second + v_sw.second) / 2 };

        if (point.second < m.second)
        {
            if (point.first < m.first)
            {
                m_sw->insert(point, id, object);
            }
            else
            {
                m_se->insert(point, id, object);
            }
        }
        else
        {
            if (point.first < m.first)
            {
                m_nw->insert(point, id, object);
            }
            else
            {
                m_ne->insert(point, id, object);
            }
        }
    }
}
template<class ID, class T>
QuadTree<ID, T>* QuadTree<ID, T>::find(const Point2D& p)
{
    if (isLeaf())
    {

        return this;
    }
    else
    {
        Point2D m = { (v_ne.first + v_se.first) / 2, (v_nw.second + v_sw.second) / 2 };

        if (p.second < m.second)
        {
            if (p.first < m.first)
            {
               m_sw->find(p);
            }
            else
            {
                m_se->find(p);
            }
        }
        else
        {
            if (p.first < m.first)
            {
                m_nw->find(p);
            }
            else
            {
                m_ne->find(p);
            }
        }
    }
}

template <class ID, class T>
void QuadTree<ID, T>::TraverseAll(std::vector<T>& allObjects)
{
    for (auto it = mObjects.begin(); it != mObjects.end(); it++)
    {
        allObjects.push_back(it->second);
    }
    return;
    if (m_ne)
    {
        m_ne->TraverseAll(allObjects);
    }
    if (m_nw)
    {
        m_nw->TraverseAll(allObjects);
    }
    if (m_sw)
    {
        m_sw->TraverseAll(allObjects);
    }
    if (m_se)
    {
        m_se->TraverseAll(allObjects);
    }
}

#endif // QUADTREE_H
