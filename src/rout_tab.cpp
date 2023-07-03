#include "rout_tab.h"

const std::string Rout_tab::retinttostr(int ret_id)
{
    switch (ret_id)
        {
            case 0:
                return "s == t";
            case 1:
                return "elem nghbrhd";
            case 2:
                return "not I(s), left base";
            case 3:
                return "not I(s), right base";
            case 4:
                return "not I(s), br(v) > l(v)";
            case 5:
                return "not I(s), not br(v) > l(v)";
            case 6:
                return "elem I(s), t < s, elem [b,nd]";
            case 7:
                return "elem I(s), t < s, not in [b,nd]";
            case 8:
                return "elem I(s), s < t, elem [nd, b]";
            case 9:
                return "elem I(s), s < t, not in [nd, b]";
            default:
                return "fehler";
        }

}

const std::list<int> Rout_tab::find_neighbors() const
{
    std::list<int> ret_acc;
    for (auto & [id, br_p] : mEntries) {
        ret_acc.push_back(id);
    }
    return ret_acc;
}

const std::tuple<int, int> Rout_tab::rout_step(int t) const
{
    if (t == this->mSelf_id) {
        return std::tuple{t, 0};
    }
    auto search_it = mEntries.find(t);
    if ( search_it != mEntries.end()) {
            return std::tuple{search_it->first, 1};
    }
    if (t < this->find_l_v() || this->find_r_v() < t)
    {
        if (this->find_l_v() == 0) {
            return std::tuple{this->find_l_v(), 2};
        }
        if (this->find_r_v() == mRight_base_id) {
            return std::tuple{this->find_r_v(), 3};
        }
        if (this->find_l_v() < this->mEntries.at(this->find_r_v()).front()) {
            return std::tuple{this->find_l_v(), 4};
        } else {
            return std::tuple{this->find_r_v(), 5};
        }
    }
    auto nd_entry = this->find_nd(t);
    auto nd_br = nd_entry->second.front();
    if (t < this->mSelf_id) {
        if (nd_br <= t && t <= nd_entry->first) {
            return std::tuple{nd_entry->first, 6};
        } else {
            return std::tuple{this->find_fd(t)->first, 7};
        }
    } else {
        if (nd_entry->first <= t && t <= nd_br) {
            return std::tuple{nd_entry->first, 8};
        } else {
            return std::tuple{this->find_fd(t)->first, 9};
        }
    }
}

int Rout_tab::find_l_v() const
{
    return mEntries.begin()->first;
}

int Rout_tab::find_r_v() const
{
    return mEntries.rbegin()->first;
}

void Rout_tab::set_brs(std::map<int,int>& brs)
{
    int port = 0;
    for (auto & [entry_id, entry_list] : mEntries) {
        auto find_it = brs.find(entry_id);
        if (find_it != brs.end()) {
            entry_list.push_back(find_it->second);
        }
        entry_list.push_back(port);
        port++;
    }
}



int Rout_tab::find_br(int v_id) const
{
    auto entry = mEntries.at(v_id);
    return entry.front();
}

bool Rout_tab::has_br(int v_id) const
{
    return mEntries.at(v_id).size() == 2;
}

const std::map<int, std::list<int>>::const_iterator 
	Rout_tab::find_nd(int t) const
{
    if (mSelf_id < t) {
        return std::next(mEntries.lower_bound(t), -1);
    } else {
        return mEntries.upper_bound(t);
    }
}

const std::map<int, std::list<int>>::const_iterator 
	Rout_tab::find_fd(int t) const
{
    if (mSelf_id < t) {
        return mEntries.upper_bound(t);
    } else {
        return std::next(mEntries.lower_bound(t), -1);
    }
}

const std::map<int, std::list<int>>& Rout_tab::entries() const
{
    return mEntries;
}

int Rout_tab::self_id() const
{
    return mSelf_id;
}

Rout_tab::Rout_tab()
{

}

Rout_tab::Rout_tab(std::list<int> nbrh, int self_id, int right_base_id)
    :mSelf_id(self_id),
      mRight_base_id(right_base_id)
{

    for (int var : nbrh) {
        std::pair<int, std::list<int>> entry_temp;
        std::get<0>(entry_temp)=var;
        mEntries.insert(entry_temp);
    }
}

const std::string Rout_tab::print_tab_ints() const
{
    std::stringstream s;
    s << "table for v " << this->mSelf_id << ":"
      << "total : " << this->count_entries() << " [";
    for (auto it = this->mEntries.begin(); 
		it != mEntries.end() ; std::advance(it,1)) {
            auto & [v_id, br_port] = (*it);
            s << "[" << v_id << ", ";
        for (auto w_id : (br_port)) {
            s << w_id << ", ";
        }
        s << "]  ";
    }
    s << "] ";
    return s.str();
}

int Rout_tab::count_entries() const
{
    int acc = 0;
    for (auto & entry : mEntries) {
        acc += 2 + std::get<1>(entry).size();
    }
    return acc;
}
