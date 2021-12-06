#pragma once

template <typename T>
class vector
{
private:
    unsigned long size = 0;
    struct ele
    {
        T *value;
        ele *prev;
        ele *next;
    };

    ele *start = nullptr;
    ele *curr = nullptr;

public:
    struct Iterator
    {
    private:
        ele *_cursor;

    public:
        Iterator(ele *c = nullptr)
        {
            this->_cursor = c;
        }

        bool operator!=(Iterator it)
        {
            return this->_cursor->value == it._cursor->value;
        }

        Iterator operator++()
        {
            return {_cursor->next};
        }

        Iterator operator--()
        {
            return {_cursor->pre};
        }

        T *&operator*()
        {
            return _cursor->value;
        }
    };

    vector()
    {
    }

    bool isEmpty()
    {
        return start == nullptr;
    }

    unsigned long length()
    {
        return size;
    }

    void add(T *value)
    {
        ele newEle = {value, curr, nullptr};
        if (start == nullptr)
        {
            start = newEle;
            curr = newEle;
        }
        else
        {
            curr->next = newEle;
            newEle->prev = curr;
            curr = newEle;
        }
        size++;
    }

    T *operator[](int pos)
    {
        ele *work = start;
        for (int i = 0; i < pos; i++)
        {
            if (work->next != nullptr)
            {
                work = work->next;
            }
            else
            {
                break;
            }
        }
        return work->value;
    }

    //vector operator+(const vector &l);
    //vector &operator=(const vector &l);
    //bool operator==(const vector &l);
    //bool operator!=(const vector &l);

    Iterator begin()
    {
        return {start};
    }

    Iterator end()
    {
        return {curr};
    }
};