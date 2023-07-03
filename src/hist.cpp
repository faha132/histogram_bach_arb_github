#include "hist.h"

const std::vector<QPoint>& Hist::getQP_Vs() const
{
    return mQP_Vs;
}

Hist::~Hist() {
}

std::vector<QPoint> create_random_vertex_list(int size)
{
    std::list<QPoint> vertex_list;
    vertex_list.push_back(QPoint(0,0));
    std::random_device rd;
    std::uniform_int_distribution<int> y_distribution(1, size);
    std::vector<int> y_vals(size);
    for (int var = 0; var < size; ++var) {
        do {
            y_vals.at(var) = y_distribution(rd) * (-5);
            } while ((var > 0) && (y_vals.at(var) == y_vals.at(var-1)));
    }
    for (int var = 0; var < (size); ++var) {
        vertex_list.push_back(QPoint(vertex_list.back().x(),
                                     y_vals.at(var)));
        vertex_list.push_back(QPoint(vertex_list.back().x()+5,
                                     vertex_list.back().y()));
    }
    vertex_list.push_back(QPoint(vertex_list.back().x(), 0));
    return std::vector<QPoint>{vertex_list.begin(), vertex_list.end()};
}

Hist::Hist(int size)
{
    if (size < 1) {
        size = 1;
    }
    mQP_Vs = create_random_vertex_list(size);
}

Hist::Hist(std::vector<std::string>& vec1)
{
    mQP_Vs.resize(vec1.size()/2);

    for (int i = 0; i < vec1.size(); ++i) {
        if (i % 2 == 0) {
            mQP_Vs.at(i/2).rx() = std::stoi(vec1.at(i));
        } else {
            mQP_Vs.at(i/2).ry() = std::stoi(vec1.at(i));
        }
    }
}

template <typename T_it>
std::list<int> visibility_point_helper(T_it ngbrhd_of_id_it,
                                       T_it last, T_it begin)
{
    std::list<int> list_acc;
    if (std::distance(ngbrhd_of_id_it,last) > 1) {
        T_it cur_v_it = std::next(ngbrhd_of_id_it);
        int y_lim = std::min(ngbrhd_of_id_it->y(), cur_v_it->y());
        for (;cur_v_it != last; std::advance(cur_v_it,1)){
            if (cur_v_it->y() < y_lim){}
            else {
                list_acc.push_back(std::abs(std::distance(begin, cur_v_it)));
                y_lim = cur_v_it->y();
                if (cur_v_it->y() > ngbrhd_of_id_it->y()) {
                    break;
                }
            }
        }
    }
    return list_acc;
}

const std::list<int> Hist::visibility_points(int v_id) const
{
    std::list<int> rnbrh_acc =
            visibility_point_helper<std::vector<QPoint>::const_iterator>(
                std::next(mQP_Vs.begin(), v_id), mQP_Vs.end(), mQP_Vs.begin()
                );
    std::list<int> lnbrh_acc =
            visibility_point_helper<
				std::vector<QPoint>::const_reverse_iterator>
                    (
            std::prev(mQP_Vs.rend(), v_id + 1), mQP_Vs.rend(), std::prev(mQP_Vs.rend(),1)
					);
    rnbrh_acc.insert(rnbrh_acc.begin(), lnbrh_acc.rbegin(), lnbrh_acc.rend());
    return rnbrh_acc;
}

void Hist::scale_invert_QP_Vs(int image_scale)
{
    for (auto & qP_V : mQP_Vs) {
        qP_V.rx() *= image_scale;
        qP_V.ry() *= (-image_scale);
    }
}

const std::vector<std::string> Hist::verts_to_strings() const
{
    std::vector<std::string> v_x_y(mQP_Vs.size()*2);
    for (size_t point_i = 0; point_i < mQP_Vs.size(); ++point_i) {
        v_x_y.at(point_i*2) = std::to_string(mQP_Vs.at(point_i).x());
        v_x_y.at(point_i*2+1) = std::to_string(mQP_Vs.at(point_i).y());
    }
    return v_x_y;
}
