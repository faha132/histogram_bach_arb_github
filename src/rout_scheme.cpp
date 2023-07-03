#include "rout_scheme.h"

const std::map<int, int> Rout_scheme::find_brs() const
{
    std::map<int,int> ret;
    for (size_t var_id = 0; var_id < (mTables.size()-1); ++var_id) {
        if (var_id % 2 == 1 ) {
            ret.insert_or_assign(mTables.at(var_id).find_l_v(), var_id);
        }
    }
    for (int var_id = (mTables.size() - 1); var_id > 0 ; --var_id) {
        if (var_id % 2 == 0) {
            ret.insert_or_assign(mTables.at(var_id).find_r_v(), var_id);
        }
    }
    return ret;
}

Rout_scheme::Rout_scheme(Hist h)
    : mTables{h.getQP_Vs().size()},
      mEntriesCount{0}, mEntriesMax{0}
{
    for (size_t var_tableNo = 0; var_tableNo < mTables.size(); ++var_tableNo) {
        mTables.at(var_tableNo) = 
			Rout_tab(h.visibility_points(var_tableNo), var_tableNo,
                                           mTables.size()-1);
    }
    auto brs = find_brs();
    for (auto& table : mTables) {
        table.set_brs(brs);
    }

    for (auto& table : mTables) {
        mEntriesCount += table.count_entries();
        mEntriesMax= std::max(table.count_entries(), mEntriesMax);
    }
    mEncodSize = std::ceil(std::log2(mTables.size()));
}

void Rout_scheme::print_tables_to_file() const
{
    std::ofstream myfile;
    myfile.open ("example.txt");
    myfile << "entries no : " << mEntriesCount
           << "\t\t entries max len : " << mEntriesMax
           << "\t\t logn : " << mEncodSize
           << "\t\t ent no enc : " << (mEntriesCount * mEncodSize)
           << "\t\t ent max enc : " << (mEntriesMax * mEncodSize)
           << std::endl;
    for (auto& tab : mTables) {
        myfile << tab.print_tab_ints() << std::endl;
    }
    myfile.close();
}

std::vector<std::string> Rout_scheme::tables_to_csv_data() const
{
    return std::vector{std::to_string(mEntriesCount),
                std::to_string(mEntriesMax)};

}

const std::vector<std::pair<std::vector<int>, double>>
    Rout_scheme::rout_data_string(int s, int t) const
{
    std::list<std::pair<std::vector<int>, double>> ret;
    int cur = s;
    int step_count = 0;
    do {
        if (step_count > std::abs(s-t)) {
            throw std::logic_error("rout_to_long, from "
                                   + std::to_string(s)
                                   + " to "
                                   + std::to_string(t))
                    ;
        }
        auto start = std::chrono::steady_clock::now();
        auto [v_id, dec_str_id] = this->mTables
                .at(cur)
                .rout_step(t);
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        int ngbr_count = mTables.at(cur).entries().size();
        std::pair<std::vector<int>, double> acc{
            {step_count,v_id, dec_str_id, ngbr_count},elapsed_seconds.count()};
        ret.push_back(acc);
        //while iterate
        step_count++;
        cur = v_id;
    } while ( cur != t);
    return {ret.begin(), ret.end()};
}

using t_i_s = std::tuple<int, std::string>;
const std::list<t_i_s> Rout_scheme::rout(int s, int t) const
{
    std::list<t_i_s> ret{std::tuple{s, "none"}};
    auto cur = std::get<0>(ret.back());
    while ( cur != t) {
        auto [v_id, dec_str_id] = this->mTables
                .at(cur)
                .rout_step(t);
        ret.push_back(t_i_s(v_id, Rout_tab::retinttostr(dec_str_id)));
        cur = std::get<0>(ret.back());
    }
    return ret;
}

const std::vector<Rout_tab>& Rout_scheme::tables() const
{
    return mTables;
}
