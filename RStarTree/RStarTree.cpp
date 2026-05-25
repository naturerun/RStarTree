#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <list>
#include <functional>
#include <stack>
#include <random>
using namespace std;
using namespace std::placeholders;

template <typename T>
struct RStarTreeNode
{
    vector<vector<pair<T, T>>> enclosed_rectangle_group;
    vector<RStarTreeNode<T>*> pointer_to_sub_tree;
    RStarTreeNode(const size_t M) :enclosed_rectangle_group(), pointer_to_sub_tree() { enclosed_rectangle_group.reserve(M + 1);  pointer_to_sub_tree.reserve(M + 1); }
};

template <typename T>
class Sort_for_interval_and_dimension
{
public:
    bool operator()(const size_t left, const size_t right)
    {
        if (left_point_or_right_point)
        {
            return enclosed_rectangle_group[left][d].first < enclosed_rectangle_group[right][d].first;
        }
        else
        {
            return enclosed_rectangle_group[left][d].second < enclosed_rectangle_group[right][d].second;
        }
    }
    Sort_for_interval_and_dimension(const vector<vector<pair<T, T>>>& e, const size_t d, bool l) :enclosed_rectangle_group(e), d(d), left_point_or_right_point(l) {}
private:
    const vector<vector<pair<T, T>>>& enclosed_rectangle_group;
    const size_t d;
    bool left_point_or_right_point;
};

template <typename T>
struct AssistInfo
{
    size_t num;
    T endpoint;
    AssistInfo() = default;
    AssistInfo(size_t n, const T& e) :num(n), endpoint(e) {}
};

template <typename T>
class RStarTree
{
public:
    void insertData(vector<pair<T, T>>& be_inserted);
    bool remove(vector<pair<T, T>>& key);
    bool isRTree();
    RStarTree(size_t dm, size_t min, size_t Max, T z, T o, size_t f) :d(dm), m(min), M(Max), zero(z), one(o), first_p(f)
    {
        if (dm == 0)
        {
            cout << "ERROR:RStarTree维度无效!" << endl;
            exit(-1);
        }

        if (min <= 1 || min > (Max + 1) / 2)
        {
            cout << "ERROR:m或M无效!" << endl;
            exit(-1);
        }

        if (f == 0 || f > M - m + 1)
        {
            cout << "ERROR:first_p值无效!" << endl;
            exit(-1);
        }
    }
private:
    RStarTreeNode<T>* root = nullptr;
    const size_t d;   //数据维度
    const size_t m;
    const size_t M;
    const T zero;
    const T one;
    const size_t first_p; //first_p在1到M-M/2+1之间，表示重新插入时要删除的条目数目
    size_t height = 0;   //R*树高度
    void insert(vector<pair<T, T>>& be_inserted, RStarTreeNode<T>* be_inserted_pointer, size_t h, vector<bool>& level_has_overflow_process);
    void re_insert(RStarTreeNode<T>* node, size_t h, vector<bool>& level_has_overflow_process, vector<pair<RStarTreeNode<T>*, size_t>>& work_stack, size_t index);
    void find_leaf(vector<pair<T, T>>& key, vector<pair<RStarTreeNode<T>*, size_t>>& work_stack, size_t& index, RStarTreeNode<T>* cur); //vector大小初始化为height
    pair<pair<vector<pair<T, T>>, vector<pair<T, T>>>, RStarTreeNode<T>*> spilit(RStarTreeNode<T>* node);
    size_t chooseSubTree(RStarTreeNode<T>* node, vector<pair<T, T>>& insert);
    void upAdjustRect(vector<pair<RStarTreeNode<T>*, size_t>>& work_stack, vector<pair<T, T>>& cur_rect, size_t index);
    RStarTreeNode<T>* findNodeInSelectedLevel(size_t h, vector<pair<T, T>>& insert, vector<pair<RStarTreeNode<T>*, size_t>>& work_stack);
    bool compute_patial_sort_result(size_t j, vector<size_t>& patial_sort_result_for_different_d, vector<size_t>& sort_result, vector<vector<pair<T, T>>>& enclosed_rectangle_group, bool compare_first);
    bool computeRect(vector<pair<T, T>>& left, vector<pair<T, T>>& right);
    void computeReactAll(vector<pair<T, T>>& left, vector<size_t>& right, size_t l, size_t r, vector<vector<pair<T, T>>>& enclosed_rectangle_group);
    T margin(vector<pair<T, T>>& cur);
    void compute_assist(size_t j, vector<size_t>& patial_sort_result_for_different_d, vector<vector<pair<T, T>>>& enclosed_rectangle_group, vector<vector<AssistInfo<T>>>& assist_info_left, vector<vector<size_t>>& rank_left);
    void compute_assist_right(size_t j, vector<size_t>& patial_sort_result_for_different_d_right, vector<vector<pair<T, T>>>& enclosed_rectangle_group, vector<vector<AssistInfo<T>>>& assist_info_right, vector<vector<size_t>>& rank_right);
    T computeArea(vector<pair<T, T>>& cur);
    bool getInterRect(vector<pair<T, T>>& be_filled, vector<pair<T, T>>& A, vector<pair<T, T>>& B);
    T compute_margin_sum(bool left_left, size_t i, vector<size_t>& sort_result_left, vector<vector<pair<T, T>>>& enclosed_rectangle_group, vector<vector<AssistInfo<T>>>& assist_info_left_left, vector<vector<size_t>>& rank_left_left, vector<vector<AssistInfo<T>>>& assist_info_left_right, vector<vector<size_t>>& rank_left_right,
        vector<pair<vector<pair<T, T>>, vector<pair<T, T>>>>& rect_pair, vector<size_t>& spilit_point_for_every_different_rect_pair);
    void findBestSpilit(vector<size_t>& spilit_point_for_every_different_rect_pair, list<pair<vector<pair<T, T>>, vector<pair<T, T>>>>& best_spilit_react, list<size_t>& best_spilit_point, T& best_inter_area, vector<pair<vector<pair<T, T>>, vector<pair<T, T>>>>& left_rect_pair);
    bool rectEqual(vector<pair<T, T>>& left, vector<pair<T, T>>& right);
    bool rectContain(vector<pair<T, T>>& judge_object, vector<pair<T, T>>& test);
    void computeAndAdjust(vector<pair<RStarTreeNode<T>*, size_t>>& work_stack, vector<vector<pair<T, T>>>& cur, size_t m, size_t index);
    void deleteAndReinsert(size_t index, vector<pair<RStarTreeNode<T>*, size_t>>& work_stack);
    bool checkRTree(RStarTreeNode<T>* cur, bool isRoot, size_t& height, vector<pair<T, T>>& enclosedRect);
    void adjustRect(vector<pair<RStarTreeNode<T>*, size_t>>& work_stack, vector<pair<T, T>>& be_inserted, size_t index);
    T findBestSpilitPlan(list<pair<vector<pair<T, T>>, vector<pair<T, T>>>>& best_spilit_react, list<size_t>& best_spilit_point, size_t& best_spilit_point_value, pair<vector<pair<T, T>>, vector<pair<T, T>>>& best_spilit_react_pair);
};

template <typename T>
void RStarTree<T>::find_leaf(vector<pair<T, T>>& key, vector<pair<RStarTreeNode<T>*, size_t>>& work_stack, size_t& index, RStarTreeNode<T>* cur)  //index可用来判断搜索是否成功
{
    if (cur->pointer_to_sub_tree.empty() == false)
    {
        for (size_t i = 0; i < cur->enclosed_rectangle_group.size(); ++i)
        {
            if (rectContain(key, cur->enclosed_rectangle_group[i]))
            {
                work_stack[index] = make_pair(cur, i);
                --index;
                find_leaf(key, work_stack, index, cur->pointer_to_sub_tree[i]);
                if (index == 0)
                    return;
            }
        }
    }
    else
    {
        for (size_t i = 0; i < cur->enclosed_rectangle_group.size(); ++i)
        {
            if (rectEqual(key, cur->enclosed_rectangle_group[i]))
            {
                work_stack[index] = make_pair(cur, i);
                return;
            }
        }
    }
    ++index;
}

template <typename T>
bool RStarTree<T>::rectEqual(vector<pair<T, T>>& left, vector<pair<T, T>>& right)
{
    for (size_t i = 0; i < d; ++i)
    {
        if (left[i].first != right[i].first || left[i].second != right[i].second)
        {
            return false;
        }
    }
    return true;
}

template <typename T>
bool RStarTree<T>::rectContain(vector<pair<T, T>>& judge_object, vector<pair<T, T>>& test)
{
    for (size_t i = 0; i < d; ++i)
    {
        if (judge_object[i].first < test[i].first || judge_object[i].second > test[i].second)
        {
            return false;
        }
    }
    return true;
}

template <typename T>
bool RStarTree<T>::computeRect(vector<pair<T, T>>& left, vector<pair<T, T>>& right)
{
    bool equal = true;
    for (size_t i = 0; i < d; ++i)
    {
        if (left[i].first > right[i].first)
        {
            left[i].first = right[i].first;
            equal = false;
        }

        if (left[i].second < right[i].second)
        {
            left[i].second = right[i].second;
            equal = false;
        }
    }
    return equal;
}

template <typename T>
void RStarTree<T>::computeAndAdjust(vector<pair<RStarTreeNode<T>*, size_t>>& work_stack, vector<vector<pair<T, T>>>& cur, size_t m, size_t index)   //
{
    vector<pair<T, T>> cur_rect = cur[0];
    for (size_t i = 1; i <= m; ++i)
    {
        computeRect(cur_rect, cur[i]);
    }

    upAdjustRect(work_stack, cur_rect, index);
}

template <typename T>
void RStarTree<T>::deleteAndReinsert(size_t index, vector<pair<RStarTreeNode<T>*, size_t>>& work_stack)
{
    if (index != 0)
    {
        {
            size_t mid = work_stack[0].second;
            vector<bool> level_has_overflow_process(height, false);
            level_has_overflow_process.back() = true;
            for (size_t j = 0; j < work_stack[0].first->enclosed_rectangle_group.size(); ++j)
            {
                if (j != mid)
                {
                    insert(work_stack[0].first->enclosed_rectangle_group[j], nullptr, 1, level_has_overflow_process);
                }
            }
            delete work_stack[0].first;
        }
    }

    for (size_t i = 1; i < index; ++i)
    {
        size_t mid = work_stack[i].second;
        vector<bool> level_has_overflow_process(height, false);
        level_has_overflow_process.back() = true;
        for (size_t j = 0; j < work_stack[i].first->enclosed_rectangle_group.size(); ++j)
        {
            if (j != mid)
            {
                insert(work_stack[i].first->enclosed_rectangle_group[j], work_stack[i].first->pointer_to_sub_tree[j], i + 1, level_has_overflow_process);
            }
        }
        delete work_stack[i].first;
    }
}

template <typename T>
bool RStarTree<T>::remove(vector<pair<T, T>>& key)
{
    if (root == nullptr)
        return false;

    vector<pair<RStarTreeNode<T>*, size_t>> work_stack(height, pair<RStarTreeNode<T>*, size_t>());
    size_t index = height - 1;    //height
    find_leaf(key, work_stack, index, root);
    if (index == height)
    {
        return false;
    }

    index = 0;
    while (true)
    {
        RStarTreeNode<T>* cur = work_stack[index].first;
        if (cur == root || cur->enclosed_rectangle_group.size() > m)
        {
            size_t i = work_stack[index].second;
            cur->enclosed_rectangle_group.erase(cur->enclosed_rectangle_group.begin() + i);
            if (index != 0)
            {
                cur->pointer_to_sub_tree.erase(cur->pointer_to_sub_tree.begin() + i);
            }

            if (cur != root)
            {
                vector<pair<T, T>>& enclosed_rectangle_cur = work_stack[index + 1].first->enclosed_rectangle_group[work_stack[index + 1].second];
                computeAndAdjust(work_stack, cur->enclosed_rectangle_group, cur->enclosed_rectangle_group.size() - 1, index + 1);
            }
            else
            {
                if (height == 1)
                {
                    if (root->enclosed_rectangle_group.empty())
                    {
                        delete root;
                        root = nullptr;
                        height = 0;
                    }
                    return true;
                }
                else
                {
                    if (root->enclosed_rectangle_group.size() == 1)
                    {
                        root = root->pointer_to_sub_tree.back();
                        delete cur;
                        --height;
                    }
                }
            }
            deleteAndReinsert(index, work_stack);
            return true;
        }
        else
        {
            ++index;
        }
    }
}

template <typename T>
class Sort_for_central_distance
{
public:
    bool operator()(const size_t left, const size_t right)
    {
        return central_distance[left] > central_distance[right];
    }
    Sort_for_central_distance(const vector<T>& e) : central_distance(e) {}
private:
    const vector<T>& central_distance;
};

template <typename T>
void RStarTree<T>::upAdjustRect(vector<pair<RStarTreeNode<T>*, size_t>>& work_stack, vector<pair<T, T>>& cur_rect, size_t index)
{
    RStarTreeNode<T>* cur = work_stack[index].first;
    enum class need_compute { need, noneed, unknown };
    vector<need_compute> need_compute(d, need_compute::unknown);

    while (true)
    {
        vector<vector<pair<T, T>>>& enclosed_rectangle_group = cur->enclosed_rectangle_group;
        size_t i;
        vector<pair<T, T>>& cur_rect_after_change = enclosed_rectangle_group[i = work_stack[index++].second];

        bool changed = false;
        for (size_t k = 0; k < d; ++k)
        {
            if (need_compute[k] == need_compute::noneed)
                continue;
            if (need_compute[k] == need_compute::unknown || need_compute[k] == need_compute::need)
            {
                if (cur_rect_after_change[k].first != cur_rect[k].first || cur_rect_after_change[k].second != cur_rect[k].second)
                {
                    cur_rect_after_change[k].first = cur_rect[k].first;
                    cur_rect_after_change[k].second = cur_rect[k].second;
                    need_compute[k] = need_compute::need;
                }
                else
                    need_compute[k] = need_compute::noneed;
            }

            if (need_compute[k] == need_compute::need)
            {
                changed = true;
                for (size_t j = 0; j < enclosed_rectangle_group.size(); ++j)
                {
                    if (j == i)
                        continue;
                    if (cur_rect[k].first > enclosed_rectangle_group[j][k].first)
                    {
                        cur_rect[k].first = enclosed_rectangle_group[j][k].first;
                    }

                    if (cur_rect[k].second < enclosed_rectangle_group[j][k].second)
                    {
                        cur_rect[k].second = enclosed_rectangle_group[j][k].second;
                    }
                }
            }
        }

        if (index == work_stack.size() || changed == false)
            break;
        cur = work_stack[index].first;
    }
}

template <typename T>
void RStarTree<T>::re_insert(RStarTreeNode<T>* node, size_t h, vector<bool>& level_has_overflow_process, vector<pair<RStarTreeNode<T>*, size_t>>& work_stack, size_t index)  //重新插入只会重插至非根节点层
{
    vector<vector<pair<T, T>>>& enclosed_rectangle_group = node->enclosed_rectangle_group;
    vector<pair<T, T>>& enclosed_rectangle_parent = work_stack[index].first->enclosed_rectangle_group[work_stack[index].second];
    vector<size_t> sort_result(M + 1);
    for (size_t j = 0; j <= M; ++j)
    {
        sort_result[j] = j;
    }
    vector<T> central_parent(d);
    for (size_t i = 0; i < d; ++i)
    {
        central_parent[i] = (enclosed_rectangle_parent[i].second + enclosed_rectangle_parent[i].first) / 2;
    }
    vector<T> central_distance(M + 1, zero);
    for (size_t i = 0; i <= M; ++i)
    {
        for (size_t j = 0; j < d; ++j)
        {
            T cental_part = (enclosed_rectangle_group[i][j].second + enclosed_rectangle_group[i][j].first) / 2;
            T t = max(cental_part, central_parent[j]) - min(cental_part, central_parent[j]);
            central_distance[i] += t * t;
        }
    }
    sort(sort_result.begin(), sort_result.end(), Sort_for_central_distance<T>(central_distance));
    vector<vector<pair<T, T>>> first_first_p(first_p);
    for (size_t i = 0; i < first_p; ++i)
    {
        first_first_p[i] = enclosed_rectangle_group[sort_result[i]];
    }

    vector<vector<pair<T, T>>> after_first_p(M + 1 - first_p);
    for (size_t i = first_p; i <= M; ++i)
    {
        after_first_p[i - first_p] = enclosed_rectangle_group[sort_result[i]];
    }

    computeAndAdjust(work_stack, after_first_p, M - first_p, index);

    vector<RStarTreeNode<T>*>& pointer = node->pointer_to_sub_tree;
    if (pointer.empty() == false)
    {
        vector<RStarTreeNode<T>*> first_first_p_p(first_p);
        vector<RStarTreeNode<T>*> after_first_p_p(M + 1 - first_p);
        for (size_t i = 0; i < first_p; ++i)
        {
            first_first_p_p[i] = pointer[sort_result[i]];
        }

        for (size_t i = first_p; i <= M; ++i)
        {
            after_first_p_p[i - first_p] = pointer[sort_result[i]];
        }
        pointer = after_first_p_p;
        enclosed_rectangle_group = after_first_p;
        for (size_t i = 0; i < first_p; ++i)
        {
            insert(first_first_p[i], first_first_p_p[i], h, level_has_overflow_process);
        }
    }
    else
    {
        enclosed_rectangle_group = after_first_p;
        for (size_t i = 0; i < first_p; ++i)
        {
            insert(first_first_p[i], nullptr, h, level_has_overflow_process);
        }
    }
}

template <typename T>
void RStarTree<T>::insertData(vector<pair<T, T>>& be_inserted)
{
    if (root == nullptr)
    {
        root = new RStarTreeNode<T>(M);
        root->enclosed_rectangle_group.push_back(be_inserted);
        height = 1;
        return;
    }
    vector<bool> level_has_overflow_process(height, false);
    level_has_overflow_process.back() = true;
    insert(be_inserted, nullptr, 1, level_has_overflow_process);
}

template<typename T>
void RStarTree<T>::adjustRect(vector<pair<RStarTreeNode<T>*, size_t>>& work_stack, vector<pair<T, T>>& be_inserted, size_t index)
{
    while (index != work_stack.size())
    {
        vector<vector<pair<T, T>>>& enclosed_rectangle_group = work_stack[index].first->enclosed_rectangle_group;
        size_t i = work_stack[index++].second;
        if (computeRect(enclosed_rectangle_group[i], be_inserted))
        {
            return;
        }
    }
}

template <typename T>
void RStarTree<T>::insert(vector<pair<T, T>>& be_inserted, RStarTreeNode<T>* be_inserted_pointer, size_t h, vector<bool>& level_has_overflow_process)
{
    vector<pair<RStarTreeNode<T>*, size_t>> work_stack(height - h);
    RStarTreeNode<T>* cur = findNodeInSelectedLevel(h, be_inserted, work_stack);
    size_t run_h = h;
    cur->enclosed_rectangle_group.push_back(be_inserted);
    if (be_inserted_pointer != nullptr)
        cur->pointer_to_sub_tree.push_back(be_inserted_pointer);
    size_t index = 0;
    while (true)
    {
        if (cur->enclosed_rectangle_group.size() <= M)
        {
            adjustRect(work_stack, be_inserted, index);
            return;
        }

        if (index != work_stack.size())
        {
            if (level_has_overflow_process[run_h] == false)
            {
                level_has_overflow_process[run_h] = true;
                re_insert(cur, run_h, level_has_overflow_process, work_stack, index);
                return;
            }
        }

        pair<pair<vector<pair<T, T>>, vector<pair<T, T>>>, RStarTreeNode<T>*> spilit_result = spilit(cur);
        if (index != work_stack.size())
        {
            vector<vector<pair<T, T>>>& enclosed_rectangle_group = work_stack[index].first->enclosed_rectangle_group;
            enclosed_rectangle_group[work_stack[index].second] = spilit_result.first.first;
            enclosed_rectangle_group.push_back(spilit_result.first.second);
            vector<RStarTreeNode<T>*>& pointer = work_stack[index].first->pointer_to_sub_tree;
            if (pointer.empty() == false)
                pointer.push_back(spilit_result.second);
            cur = work_stack[index++].first;
            ++run_h;
        }
        else
        {
            root = new RStarTreeNode<T>(M);
            root->enclosed_rectangle_group.push_back(spilit_result.first.first);
            root->enclosed_rectangle_group.push_back(spilit_result.first.second);
            root->pointer_to_sub_tree.push_back(cur);
            root->pointer_to_sub_tree.push_back(spilit_result.second);
            ++height;
            level_has_overflow_process.push_back(true);
            return;
        }
    }
}

template <typename T>
RStarTreeNode<T>* RStarTree<T>::findNodeInSelectedLevel(size_t h, vector<pair<T, T>>& be_inserted, vector<pair<RStarTreeNode<T>*, size_t>>& work_stack)
{
    if (h == 0)
    {
        cerr << "ERRO0R:无效的高度参数" << endl;
        exit(-1);
    }

    if (height == 0)
    {
        cerr << "空树，搜索失败" << endl;
        exit(-1);
    }
    if (h > height)
    {
        cerr << "ERRO0R:无效的高度参数" << endl;
        exit(-1);
    }

    RStarTreeNode<T>* p = root;
    size_t stack_index = work_stack.size();
    while (stack_index != 0)
    {
        size_t i = chooseSubTree(p, be_inserted);
        work_stack[--stack_index] = make_pair(p, i);
        p = p->pointer_to_sub_tree[i];
    }
    return p;
}

template <typename T>
void updateRelatedInfo(size_t i, size_t min_i, vector<size_t>& min_i_area_diff, vector<T>& min_area, T& min_area_diff, const T& a, const T& area_diff)
{
    if (i == 0 || min_area_diff > area_diff || min_area_diff == area_diff)
    {
        if (i == 0 || min_area_diff > area_diff)
        {
            if (i != 0)
            {
                min_i_area_diff.clear();
                min_area.clear();
            }
            min_area_diff = area_diff;
        }
        min_i_area_diff.push_back(min_i);
        min_area.push_back(a);
    }
}
template <typename T>
size_t RStarTree<T>::chooseSubTree(RStarTreeNode<T>* node, vector<pair<T, T>>& insert)
{
    vector<vector<pair<T, T>>>& enclosed_rectangle_group = node->enclosed_rectangle_group;
    vector<size_t> min_i;  min_i.reserve(enclosed_rectangle_group.size());
    vector<size_t> min_i_area_diff;
    vector<T> min_area;
    if (node->pointer_to_sub_tree[0]->pointer_to_sub_tree.empty())
    {
        vector<vector<T>> inter_section_area(enclosed_rectangle_group.size(), vector<T>(enclosed_rectangle_group.size()));
        T min_add_overlap;
        vector<vector<pair<T, T>>> rect_after_insert;  rect_after_insert.reserve(enclosed_rectangle_group.size());
        vector<bool> contain; contain.reserve(enclosed_rectangle_group.size());
        for (size_t i = 0; i < enclosed_rectangle_group.size() - 1; ++i)
        {
            T sum_area_value = zero;
            for (size_t j = i + 1; j < enclosed_rectangle_group.size(); ++j)
            {
                vector<pair<T, T>> be_filled(d);
                if (getInterRect(be_filled, enclosed_rectangle_group[i], enclosed_rectangle_group[j]))
                {
                    T area = computeArea(be_filled);
                    sum_area_value += area;
                    inter_section_area[i][j] = area;
                }
                else
                {
                    inter_section_area[i][j] = zero;
                }
            }

            for (size_t j = 0; j < i; ++j)
            {
                sum_area_value += inter_section_area[j][i];
            }

            vector<pair<T, T>> R = enclosed_rectangle_group[i];
            T add_overlap;
            bool temp;
            if ((temp = computeRect(R, insert)) == false)
            {
                T sum_area_value_after_insert = zero;
                for (size_t j = 0; j < enclosed_rectangle_group.size(); ++j)
                {
                    if (j == i)
                        continue;
                    vector<pair<T, T>> be_filled(d);
                    if (getInterRect(be_filled, enclosed_rectangle_group[j], R))
                        sum_area_value_after_insert += computeArea(be_filled);
                }
                add_overlap = sum_area_value_after_insert - sum_area_value;
            }
            else
            {
                add_overlap = zero;
            }

            if (i == 0 || min_add_overlap > add_overlap || min_add_overlap == add_overlap)
            {
                if (i == 0 || min_add_overlap > add_overlap)
                {
                    if (i != 0)
                    {
                        min_i.clear();
                        rect_after_insert.clear();
                        contain.clear();
                    }
                    min_add_overlap = add_overlap;
                }
                min_i.push_back(i);
                if (temp == false)
                {
                    rect_after_insert.push_back(R);
                }
                contain.push_back(temp);
            }
        }

        size_t j = 0;
        T min_area_diff;
        min_i_area_diff.reserve(min_i.size());
        min_area.reserve(min_i.size());
        for (size_t i = 0; i < min_i.size(); ++i)
        {
            T area_diff;
            T a = computeArea(enclosed_rectangle_group[min_i[i]]);
            if (contain[i] == false)
            {
                area_diff = computeArea(rect_after_insert[j++]) - a;
            }
            else
            {
                area_diff = zero;
            }
            updateRelatedInfo(i, min_i[i], min_i_area_diff, min_area, min_area_diff, a, area_diff);
        }
    }
    else
    {
        T min_area_diff;
        min_i_area_diff.reserve(enclosed_rectangle_group.size());
        min_area.reserve(enclosed_rectangle_group.size());
        for (size_t i = 0; i < enclosed_rectangle_group.size(); ++i)
        {
            T area_diff;
            bool temp;
            vector<pair<T, T>> be_filled = enclosed_rectangle_group[i];
            T a;
            if ((temp = computeRect(be_filled, insert)) == false)
            {
                area_diff = computeArea(be_filled) - (a = computeArea(enclosed_rectangle_group[i]));
            }
            else
            {
                area_diff = zero;
            }
            updateRelatedInfo(i, i, min_i_area_diff, min_area, min_area_diff, a, area_diff);
        }
    }
    size_t index = min_i_area_diff[0];
    T m_area_value = min_area[0];
    for (size_t i = 1; i < min_i_area_diff.size(); ++i)
    {
        if (m_area_value > min_area[i])
        {
            m_area_value = min_area[i];
            index = min_i_area_diff[i];
        }
    }
    return index;
}

template <typename T>
bool RStarTree<T>::compute_patial_sort_result(size_t j, vector<size_t>& patial_sort_result_for_different_d, vector<size_t>& sort_result, vector<vector<pair<T, T>>>& enclosed_rectangle_group, bool compare_first)
{
    patial_sort_result_for_different_d.insert(patial_sort_result_for_different_d.end(), sort_result.begin() + m, sort_result.begin() + M + 1 - m + 1);
    vector<size_t> patial_sort_result_for_after_m(sort_result.begin() + m, sort_result.end());
    sort(patial_sort_result_for_after_m.begin(), patial_sort_result_for_after_m.end(), Sort_for_interval_and_dimension<T>(enclosed_rectangle_group, j, compare_first));

    size_t low = 0;
    size_t up = M - m;
    size_t i = 0;
    size_t k;
    if (compare_first)
    {
        sort(patial_sort_result_for_different_d.begin(), patial_sort_result_for_different_d.end(), Sort_for_interval_and_dimension<T>(enclosed_rectangle_group, j, compare_first));
        if (patial_sort_result_for_after_m[low] != patial_sort_result_for_different_d.front())
            return true;


        for (k = low + 1, ++i; i < patial_sort_result_for_different_d.size(); ++k)
        {
            if (patial_sort_result_for_after_m[k] != patial_sort_result_for_different_d[i])
                break;
            ++i;
        }
    }
    else
    {
        sort(patial_sort_result_for_different_d.begin(), patial_sort_result_for_different_d.end(), bind(Sort_for_interval_and_dimension<T>(enclosed_rectangle_group, j, compare_first), _2, _1));
        if (patial_sort_result_for_after_m[up] != patial_sort_result_for_different_d.front())
            return true;

        for (k = up - 1, ++i; i < patial_sort_result_for_different_d.size(); --k)
        {
            if (patial_sort_result_for_after_m[k] != patial_sort_result_for_different_d[i])
                break;
            ++i;
        }

    }
    patial_sort_result_for_different_d.erase(patial_sort_result_for_different_d.begin() + i, patial_sort_result_for_different_d.end());
    patial_sort_result_for_different_d.push_back(patial_sort_result_for_after_m[k]);
    if (compare_first)
        reverse(patial_sort_result_for_different_d.begin(), patial_sort_result_for_different_d.end());
    return false;
}

template <typename T>
T RStarTree<T>::margin(vector<pair<T, T>>& cur)
{
    T sum = zero;
    for (size_t i = 0; i < d; ++i)
    {
        sum += cur[i].second - cur[i].first;
    }
    return sum;
}
template <typename T>
void RStarTree<T>::computeReactAll(vector<pair<T, T>>& left, vector<size_t>& right, size_t l, size_t r, vector<vector<pair<T, T>>>& enclosed_rectangle_group)
{
    left = enclosed_rectangle_group[right[l]];
    for (size_t i = l + 1; i <= r; ++i)
    {
        computeRect(left, enclosed_rectangle_group[right[i]]);
    }
}

template <typename T>
void RStarTree<T>::compute_assist(size_t j, vector<size_t>& patial_sort_result_for_different_d, vector<vector<pair<T, T>>>& enclosed_rectangle_group, vector<vector<AssistInfo<T>>>& assist_info_left, vector<vector<size_t>>& rank_left)
{
    size_t k = 0;
    while (k < patial_sort_result_for_different_d.size())
    {
        assist_info_left[j].push_back(AssistInfo<T>(1, enclosed_rectangle_group[patial_sort_result_for_different_d[k]][j].first));
        while (true)
        {
            rank_left[j][patial_sort_result_for_different_d[k]] = assist_info_left[j].size() - 1;

            if (k >= patial_sort_result_for_different_d.size() - 1 || enclosed_rectangle_group[patial_sort_result_for_different_d[k]][j].first != enclosed_rectangle_group[patial_sort_result_for_different_d[k + 1]][j].first)
            {
                ++k;
                break;
            }
            ++assist_info_left[j].back().num;
            ++k;
        }
    }
}

template <typename T>
void RStarTree<T>::compute_assist_right(size_t j, vector<size_t>& patial_sort_result_for_different_d_right, vector<vector<pair<T, T>>>& enclosed_rectangle_group, vector<vector<AssistInfo<T>>>& assist_info_right, vector<vector<size_t>>& rank_right)
{
    size_t k = 0;
    while (k < patial_sort_result_for_different_d_right.size())
    {
        assist_info_right[j].push_back(AssistInfo<T>(1, enclosed_rectangle_group[patial_sort_result_for_different_d_right[k]][j].second));
        while (true)
        {
            rank_right[j][patial_sort_result_for_different_d_right[k]] = assist_info_right[j].size() - 1;
            if (k >= patial_sort_result_for_different_d_right.size() - 1 || enclosed_rectangle_group[patial_sort_result_for_different_d_right[k]][j].second != enclosed_rectangle_group[patial_sort_result_for_different_d_right[k + 1]][j].second)
            {
                ++k;
                break;
            }
            ++assist_info_right[j].back().num;
            ++k;
        }
    }
}
template <typename T>
T RStarTree<T>::compute_margin_sum(bool left_left, size_t i, vector<size_t>& sort_result_left, vector<vector<pair<T, T>>>& enclosed_rectangle_group, vector<vector<AssistInfo<T>>>& assist_info_left_left, vector<vector<size_t>>& rank_left_left, vector<vector<AssistInfo<T>>>& assist_info_left_right, vector<vector<size_t>>& rank_left_right,
    vector<pair<vector<pair<T, T>>, vector<pair<T, T>>>>& rect_pair, vector<size_t>& spilit_point_for_every_different_rect_pair)
{
    vector<pair<T, T>> rect_left;
    computeReactAll(rect_left, sort_result_left, 0, m - 1, enclosed_rectangle_group);
    vector<pair<T, T>> rect_right;
    computeReactAll(rect_right, sort_result_left, m, M, enclosed_rectangle_group);
    T cur_margin_sum_for_rect_pair_left = margin(rect_left);
    T cur_margin_sum_for_rect_pair_right = margin(rect_right);
    T margin_sum_value = cur_margin_sum_for_rect_pair_left + cur_margin_sum_for_rect_pair_right;
    rect_pair.push_back(make_pair(rect_left, rect_right));
    spilit_point_for_every_different_rect_pair.push_back(m - 1);
    T cur_margin_sum_for_rect_pair = margin_sum_value;
    for (size_t j = m; j <= M - m; ++j)
    {
        bool equal_left = computeRect(rect_left, enclosed_rectangle_group[sort_result_left[j]]);
        bool equal_right = true;
        for (size_t q = 0; q < d; ++q)
        {
            if (q == i)
            {
                if (assist_info_left_left[q].empty())  ///注意
                {
                    if (left_left && enclosed_rectangle_group[sort_result_left[j]][i].first != enclosed_rectangle_group[sort_result_left[j + 1]][i].first)
                    {
                        rect_right[i].first = enclosed_rectangle_group[sort_result_left[j + 1]][i].first;
                        equal_right = false;
                    }
                    if (assist_info_left_right[q].empty())
                        continue;
                }
            }

            if (assist_info_left_left[q].empty() == false)
            {
                if (rank_left_left[q][sort_result_left[j]] != M + 1)
                {
                    --assist_info_left_left[q][rank_left_left[q][sort_result_left[j]]].num;
                    if (enclosed_rectangle_group[sort_result_left[j]][q].first == rect_right[q].first)
                    {
                        if (assist_info_left_left[q][rank_left_left[q][sort_result_left[j]]].num == 0)
                        {
                            size_t z = rank_left_left[q][sort_result_left[j]] + 1;
                            for (; ; ++z)
                            {
                                if (assist_info_left_left[q][z].num != 0)
                                    break;
                            }
                            rect_right[q].first = assist_info_left_left[q][z].endpoint;
                            equal_right = false;
                        }
                    }
                }
            }

            if (assist_info_left_right[q].empty() == false)
            {
                if (rank_left_right[q][sort_result_left[j]] != M + 1)
                {
                    --assist_info_left_right[q][rank_left_right[q][sort_result_left[j]]].num;
                    if (enclosed_rectangle_group[sort_result_left[j]][q].second == rect_right[q].second)
                    {
                        if (assist_info_left_right[q][rank_left_right[q][sort_result_left[j]]].num == 0)
                        {
                            size_t z = rank_left_right[q][sort_result_left[j]] - 1;
                            for (; ; --z)
                            {
                                if (assist_info_left_left[q][z].num != 0)
                                    break;
                            }
                            rect_right[q].second = assist_info_left_right[q][z].endpoint;
                            equal_right = false;
                        }
                    }
                }
            }
        }
        if (equal_left)
        {
            if (equal_right == false)
            {
                cur_margin_sum_for_rect_pair_right = margin(rect_right);
            }
        }
        else
        {
            if (equal_right)
            {
                cur_margin_sum_for_rect_pair_left = margin(rect_left);
            }
            else
            {
                cur_margin_sum_for_rect_pair_left = margin(rect_left);
                cur_margin_sum_for_rect_pair_right = margin(rect_right);
            }
        }

        if (equal_left == false || equal_right == false)
        {
            cur_margin_sum_for_rect_pair = cur_margin_sum_for_rect_pair_left + cur_margin_sum_for_rect_pair_right;
            rect_pair.push_back(make_pair(rect_left, rect_right));
            spilit_point_for_every_different_rect_pair.push_back(j);
        }
        margin_sum_value += cur_margin_sum_for_rect_pair;
    }
    return margin_sum_value;
}

template <typename T>
pair<size_t, size_t> getIntersection(pair<T, T> l, pair<T, T> j)
{
    if (l.second < j.first)
    {
        return { j.first, l.second };
    }
    else if (l.first > j.second)
    {
        return { l.first, j.second };
    }
    if (j.first <= l.first)
    {
        if (j.second < l.second)
            return { l.first, j.second };
        return l;
    }

    if (j.second <= l.second)
        return j;
    return { j.first, l.second };
}

template <typename T>
T RStarTree<T>::computeArea(vector<pair<T, T>>& cur)
{
    bool zero_s = true;
    T add_area = one;
    for (size_t i = 0; i < d; ++i)
    {
        if (cur[i].first < cur[i].second)
        {
            add_area *= cur[i].second - cur[i].first;
            zero_s = false;
        }
    }
    if (zero_s)
        return zero;
    return add_area;
}

template <typename T>
bool RStarTree<T>::getInterRect(vector<pair<T, T>>& be_filled, vector<pair<T, T>>& A, vector<pair<T, T>>& B)
{
    for (size_t i = 0; i < d; ++i)
    {
        pair<T, T> inter = getIntersection(A[i], B[i]);
        if (inter.first > inter.second)
            return false;
        be_filled[i] = inter;
    }
    return true;
}

template <typename T>
void RStarTree<T>::findBestSpilit(vector<size_t>& spilit_point_for_every_different_rect_pair, list<pair<vector<pair<T, T>>, vector<pair<T, T>>>>& best_spilit_react, list<size_t>& best_spilit_point, T& best_inter_area, vector<pair<vector<pair<T, T>>, vector<pair<T, T>>>>& left_rect_pair)
{
    for (size_t i = 0; i < left_rect_pair.size(); ++i)
    {
        vector<pair<T, T>> be_filled(d);
        T area;
        if (getInterRect(be_filled, left_rect_pair[i].first, left_rect_pair[i].second))
        {
            area = computeArea(be_filled);
        }
        else
        {
            area = zero;
        }

        if (i == 0 || area < best_inter_area || area == best_inter_area)
        {
            if (i == 0 || area < best_inter_area)
            {
                if (i != 0)
                {
                    best_spilit_point.clear();
                    best_spilit_react.clear();
                }
                best_inter_area = area;
            }
            best_spilit_point.push_back(spilit_point_for_every_different_rect_pair[i]);
            best_spilit_react.push_back(make_pair(left_rect_pair[i].first, left_rect_pair[i].second));
        }
    }
}

template <typename T>
T RStarTree<T>::findBestSpilitPlan(list<pair<vector<pair<T, T>>, vector<pair<T, T>>>>& best_spilit_react, list<size_t>& best_spilit_point, size_t& best_spilit_point_value, pair<vector<pair<T, T>>, vector<pair<T, T>>>& best_spilit_react_pair)
{
    T best_area_sum;
    list<size_t>::iterator run2 = best_spilit_point.begin();
    typename list<pair<vector<pair<T, T>>, vector<pair<T, T>>>>::iterator run = best_spilit_react.begin();

    best_area_sum = computeArea(run->first) + computeArea(run->second);
    best_spilit_point_value = best_spilit_point.front();
    best_spilit_react_pair = *run;
    for (++run, ++run2; run != best_spilit_react.end(); ++run, ++run2)
    {
        T a = computeArea(run->first) + computeArea(run->second);
        if (a < best_area_sum)
        {
            best_area_sum = a;
            best_spilit_point_value = *run2;
            best_spilit_react_pair = *run;
        }
    }
    return best_area_sum;
}

template <typename T>
pair<pair<vector<pair<T, T>>, vector<pair<T, T>>>, RStarTreeNode<T>*> RStarTree<T>::spilit(RStarTreeNode<T>* node)  //注意该函数中没有删除node节点
{
    T d_margin;
    vector<size_t> sort_result_best_left;
    vector<size_t> sort_result_best_right;
    vector<pair<vector<pair<T, T>>, vector<pair<T, T>>>> left_rect_pair;
    vector<pair<vector<pair<T, T>>, vector<pair<T, T>>>> right_rect_pair;
    vector<size_t> spilit_point_for_every_different_rect_pair_left_best;
    vector<size_t> spilit_point_for_every_different_rect_pair_right_best;
    vector<vector<pair<T, T>>>& enclosed_rectangle_group = node->enclosed_rectangle_group;
    for (size_t i = 0; i < d; ++i)
    {
        vector<size_t> sort_result_left(M + 1);
        vector<size_t> sort_result_right(M + 1);
        for (size_t j = 0; j <= M; ++j)
        {
            sort_result_left[j] = j;
            sort_result_right[j] = j;
        }
        sort(sort_result_left.begin(), sort_result_left.end(), Sort_for_interval_and_dimension<T>(enclosed_rectangle_group, i, true));
        sort(sort_result_right.begin(), sort_result_right.end(), Sort_for_interval_and_dimension<T>(enclosed_rectangle_group, i, false));

        vector<vector<AssistInfo<T>>> assist_info_left_left(d, vector<AssistInfo<T>>());
        vector<vector<AssistInfo<T>>> assist_info_left_right(d, vector<AssistInfo<T>>());
        vector<vector<size_t>> rank_left_left(d, vector<size_t>(M + 1, M + 1));
        vector<vector<size_t>> rank_left_right(d, vector<size_t>(M + 1, M + 1));
        vector<vector<size_t>> rank_right_left(d, vector<size_t>(M + 1, M + 1));
        vector<vector<size_t>> rank_right_right(d, vector<size_t>(M + 1, M + 1));
        vector<vector<AssistInfo<T>>> assist_info_right_left(d, vector<AssistInfo<T>>());
        vector<vector<AssistInfo<T>>> assist_info_right_right(d, vector<AssistInfo<T>>());

        for (size_t j = 0; j < d; ++j)
        {
            if (j != i)
            {
                vector<size_t> patial_sort_result_for_different_d_left_left;
                if (compute_patial_sort_result(j, patial_sort_result_for_different_d_left_left, sort_result_left, enclosed_rectangle_group, true) == false)
                {
                    compute_assist(j, patial_sort_result_for_different_d_left_left, enclosed_rectangle_group, assist_info_left_left, rank_left_left);
                }
            }
            vector<size_t> patial_sort_result_for_different_d_left_right;
            if (compute_patial_sort_result(j, patial_sort_result_for_different_d_left_right, sort_result_left, enclosed_rectangle_group, false) == false)
                compute_assist(j, patial_sort_result_for_different_d_left_right, enclosed_rectangle_group, assist_info_left_right, rank_left_right);

            if (j != i)
            {
                vector<size_t> patial_sort_result_for_different_d_right_right;
                if (compute_patial_sort_result(j, patial_sort_result_for_different_d_right_right, sort_result_right, enclosed_rectangle_group, false) == false)
                    compute_assist_right(j, patial_sort_result_for_different_d_right_right, enclosed_rectangle_group, assist_info_right_right, rank_right_right);
            }
            vector<size_t> patial_sort_result_for_different_d_right_left;
            if (compute_patial_sort_result(j, patial_sort_result_for_different_d_right_left, sort_result_right, enclosed_rectangle_group, true) == false)
                compute_assist_right(j, patial_sort_result_for_different_d_right_left, enclosed_rectangle_group, assist_info_right_left, rank_right_left);
        }

        vector<pair<vector<pair<T, T>>, vector<pair<T, T>>>> left_rect_pair_cur;  left_rect_pair_cur.reserve(M + 2 - 2 * m);
        vector<pair<vector<pair<T, T>>, vector<pair<T, T>>>> right_rect_pair_cur; right_rect_pair_cur.reserve(M + 2 - 2 * m);
        vector<size_t> spilit_point_for_every_different_rect_pair_left;   spilit_point_for_every_different_rect_pair_left.reserve(M + 2 - 2 * m);
        vector<size_t> spilit_point_for_every_different_rect_pair_right;  spilit_point_for_every_different_rect_pair_right.reserve(M + 2 - 2 * m);
        T margin_sum_value = compute_margin_sum(true, i, sort_result_left, enclosed_rectangle_group, assist_info_left_left, rank_left_left, assist_info_left_right, rank_left_right, left_rect_pair_cur, spilit_point_for_every_different_rect_pair_left);
        margin_sum_value += compute_margin_sum(false, i, sort_result_right, enclosed_rectangle_group, assist_info_right_left, rank_right_left, assist_info_right_right, rank_right_right, right_rect_pair_cur, spilit_point_for_every_different_rect_pair_right);
        if (i == 0 || d_margin > margin_sum_value)
        {
            d_margin = margin_sum_value;
            swap(sort_result_best_left, sort_result_left);
            swap(sort_result_best_right, sort_result_right);
            swap(left_rect_pair, left_rect_pair_cur);
            swap(right_rect_pair, right_rect_pair_cur);
            swap(spilit_point_for_every_different_rect_pair_left_best, spilit_point_for_every_different_rect_pair_left);
            swap(spilit_point_for_every_different_rect_pair_right_best, spilit_point_for_every_different_rect_pair_right);
        }
    }

    list<pair<vector<pair<T, T>>, vector<pair<T, T>>>> best_spilit_react;
    list<size_t> best_spilit_point;
    T best_inter_area;
    findBestSpilit(spilit_point_for_every_different_rect_pair_left_best, best_spilit_react, best_spilit_point, best_inter_area, left_rect_pair);


    list<pair<vector<pair<T, T>>, vector<pair<T, T>>>> best_spilit_react_right;
    list<size_t> best_spilit_point_right;
    T best_inter_area_right;
    findBestSpilit(spilit_point_for_every_different_rect_pair_right_best, best_spilit_react_right, best_spilit_point_right, best_inter_area_right, right_rect_pair);

    size_t best_spilit_point_value;
    pair<vector<pair<T, T>>, vector<pair<T, T>>> best_spilit_react_pair;
    vector<size_t> sort_result;
    if (best_inter_area < best_inter_area_right)
    {
        findBestSpilitPlan(best_spilit_react, best_spilit_point, best_spilit_point_value, best_spilit_react_pair);
        sort_result = sort_result_best_left;
    }
    else if (best_inter_area > best_inter_area_right)
    {
        findBestSpilitPlan(best_spilit_react_right, best_spilit_point_right, best_spilit_point_value, best_spilit_react_pair);
        sort_result = sort_result_best_right;
    }
    else
    {
        size_t best_spilit_point_value_right;
        pair<vector<pair<T, T>>, vector<pair<T, T>>> best_spilit_react_pair_right;
        T a_s_l = findBestSpilitPlan(best_spilit_react, best_spilit_point, best_spilit_point_value, best_spilit_react_pair);
        T a_s_r = findBestSpilitPlan(best_spilit_react_right, best_spilit_point_right, best_spilit_point_value_right, best_spilit_react_pair_right);
        if (a_s_r < a_s_l)
        {
            best_spilit_point_value = best_spilit_point_value_right;
            swap(best_spilit_react_pair, best_spilit_react_pair_right);
            swap(sort_result, sort_result_best_right);
        }
        else
        {
            swap(sort_result, sort_result_best_left);
        }
    }

    vector<vector<pair<T, T>>> left;
    vector<RStarTreeNode<T>*> left_p;
    RStarTreeNode<T>* right = new RStarTreeNode<T>(M);
    for (size_t i = 0; i <= best_spilit_point_value; ++i)
    {
        left.push_back(enclosed_rectangle_group[sort_result[i]]);
        if (node->pointer_to_sub_tree.empty() == false)
        {
            left_p.push_back(node->pointer_to_sub_tree[sort_result[i]]);
        }
    }

    for (size_t i = best_spilit_point_value + 1; i <= M; ++i)
    {
        right->enclosed_rectangle_group.push_back(enclosed_rectangle_group[sort_result[i]]);
        if (node->pointer_to_sub_tree.empty() == false)
        {
            right->pointer_to_sub_tree.push_back(node->pointer_to_sub_tree[sort_result[i]]);
        }
    }
    enclosed_rectangle_group = left;
    node->pointer_to_sub_tree = left_p;
    return make_pair(best_spilit_react_pair, right);
}

template <typename T>
bool RStarTree<T>::checkRTree(RStarTreeNode<T>* cur, bool isRoot, size_t& height, vector<pair<T, T>>& enclosedRect)
{
    if (isRoot)
    {
        if (cur == nullptr)
            return true;

        if (cur->pointer_to_sub_tree.empty())
        {
            if (cur->enclosed_rectangle_group.empty() || cur->enclosed_rectangle_group.size() > M)
            {
                return false;
            }
        }
        else
        {
            if (cur->enclosed_rectangle_group.size() < 2 || cur->enclosed_rectangle_group.size() > M)
            {
                return false;
            }
        }
    }
    else
    {
        if (cur->enclosed_rectangle_group.size() < m || cur->enclosed_rectangle_group.size() > M)
        {
            cout << "大小越界";
            system("pause");
            return false;
        }
    }

    if (cur->pointer_to_sub_tree.empty() == false)
    {
        if (cur->enclosed_rectangle_group.size() != cur->pointer_to_sub_tree.size())
            return false;
        size_t h;
        for (size_t i = 0; i < cur->pointer_to_sub_tree.size(); ++i)
        {
            if (checkRTree(cur->pointer_to_sub_tree[i], false, h, enclosedRect) == false)
                return false;

            if (i == 0)
            {
                height = h;
            }
            else if (h != height)
            {
                cout << "高度不等";
                system("pause");
                return false;
            }
            if (rectEqual(cur->enclosed_rectangle_group[i], enclosedRect) == false)
            {
                cout << "矩形不完全包围";
                system("pause");
                return false;
            }
        }
    }

    if (isRoot == false)
    {
        if (cur->pointer_to_sub_tree.empty())
        {
            enclosedRect = cur->enclosed_rectangle_group.back();
            height = 0;
        }

        for (size_t i = cur->enclosed_rectangle_group.size() - 2; ; --i)
        {
            computeRect(enclosedRect, cur->enclosed_rectangle_group[i]);
            if (i == 0)
                break;
        }
        ++height;
    }
    return true;
}
ostream& operator<<(ostream& c, vector<pair<int, int>>& cur)
{
    for (size_t i = 0; i < cur.size(); ++i)
    {
        cout << "[" << cur[i].first << "," << cur[i].second << "] ";
    }
    return c;
}
template <typename T>
bool RStarTree<T>::isRTree()
{
    size_t height;
    vector<pair<T, T>> enclosedRect;
    return checkRTree(root, true, height, enclosedRect);
}
int main()
{
    const int N = 20;
    vector<int> test(N);
    for (size_t i = 0; i < N; ++i)
    {
        test[i] = i + 1;
    }

    vector<pair<int, int>> data_rect(N * (N - 1) / 2);
    size_t j = 0;
    for (size_t i = 0; i < test.size() - 1; ++i)
    {
        for (size_t k = i + 1; k < test.size(); ++k)
        {
            data_rect[j++] = make_pair(test[i], test[k]);
        }
    }
    shuffle(data_rect.begin(), data_rect.end(), default_random_engine());
    vector<vector<pair<int, int>>> insert(data_rect.size() * (data_rect.size() - 1) / 2, vector<pair<int, int>>(2));
    j = 0;
    for (size_t i = 0; i < data_rect.size() - 1; ++i)
    {
        for (size_t k = i + 1; k < data_rect.size(); ++k)
        {
            insert[j][0] = data_rect[i];
            insert[j++][1] = data_rect[k];
        }
    }

    RStarTree<int> obj(2, 2, 3, 0, 1, 2);
    for (size_t i = 0; i < insert.size(); ++i)
    {
        if (i == 70)
            system("pause");
        cout << "插入" << endl;
        cout << insert[i] << endl;
        obj.insertData(insert[i]);
        if (obj.isRTree() == false)
        {
            cout << "Error:不是R树!" << endl;
            exit(-1);
        }
        else
        {
            cout << "当前树是R树" << endl;
        }
    }

    for (size_t i = 0; i < insert.size(); ++i)
    {
        cout << "删除" << endl;
        cout << insert[i] << endl;
        if (obj.remove(insert[i]))
        {
            cout << "删除成功" << endl;
        }
        else
        {
            cout << "删除失败" << endl;
            exit(-1);
        }

        if (obj.isRTree() == false)
        {
            cout << "Error:不是R树!" << endl;
            exit(-1);
        }
        else
        {
            cout << "当前树是R树" << endl;
        }
    }
    return 0;
}
