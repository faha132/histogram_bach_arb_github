#include "graphicsscene.h"


GraphicsScene::GraphicsScene(QObject *parent)
    : QGraphicsScene{parent},
      mHist(9),
      mRScheme(mHist)
{
    set_hist(mHist);
}


void GraphicsScene::set_hist(Hist hist)
{
    mRScheme = Rout_scheme(hist);
    mBr_map = mRScheme.find_brs();
    hist.scale_invert_QP_Vs(mImage_scale);
    mHist = hist;
    mTexts_ptrs.resize(mHist.getQP_Vs().size());
    for (size_t var = 0; var < mHist.getQP_Vs().size(); ++var)
    {
        mTexts_ptrs.at(var) = this->addSimpleText(std::to_string(var).data());
        mTexts_ptrs.at(var)->setPos(mHist.getQP_Vs().at(var));
    }
    auto qps = mHist.getQP_Vs();
    mHist_Itm = this->addPolygon(QPolygon{qps.begin(), qps.end()});
    mRScheme.print_tables_to_file();

}

void GraphicsScene::highlight_v(int v_id)
{
    QPen pen(Qt::blue);
    this->reset_highlight();
    auto v_id_tab = mRScheme.tables().at(v_id);
    auto to_highlght_v_ints = {v_id_tab.find_l_v(),
                               v_id_tab.find_r_v()
                                  };
    for (auto w_id : to_highlght_v_ints) {
        QPoint p = mHist.getQP_Vs().at(w_id);
        mGra_Itms_ptrs.append(
                    this->addEllipse(
                        p.x()-mImage_scale/2, p.y()-mImage_scale/2,
                        mImage_scale, mImage_scale, pen
                        )
                    );
        mTextNoChanged.push_back(w_id);
    }
    auto br_it = mBr_map.find(v_id);
    if (br_it != mBr_map.end()) {
        QPoint p = mHist.getQP_Vs().at(br_it->second);
        mGra_Itms_ptrs.append(
                    this->addEllipse(
                        p.x()-mImage_scale/2, p.y()-mImage_scale/2,
                        mImage_scale, mImage_scale, pen
                        )
                    );
        mTextNoChanged.push_back(br_it->second);
	}
    auto highlight_caption = std::array{"l", "r", "br"};
    for (size_t var = 0; var < mTextNoChanged.size(); ++var) {
        auto text_itm_ptr = mTexts_ptrs.at(mTextNoChanged.at(var));
        text_itm_ptr->setText(text_itm_ptr->text() + highlight_caption[var]);
    }
    for (auto v_n : v_id_tab.entries()){
        QLine line(
                    mHist.getQP_Vs().at(v_id_tab.self_id()),
                    mHist.getQP_Vs().at(std::get<0>(v_n))
                    );
        mGra_Itms_ptrs.append(this->addLine(line, pen));
    }
}

 void GraphicsScene::remove_hist()
{
    this->reset_highlight();
    this->reset_rout();
    this->removeItem(mHist_Itm);
    for (auto text : mTexts_ptrs) {
        this->removeItem(text);
    }
    this->mSh_path = nullptr;
}

void GraphicsScene::reset_highlight()
{
    for (auto item : mGra_Itms_ptrs) {
        this->removeItem(item);
    }
    mGra_Itms_ptrs.clear();
    for (auto textNo : mTextNoChanged) {
        mTexts_ptrs.at(textNo)->setText(
                    std::to_string(textNo).data()
                    );
    }
    mTextNoChanged.clear();
}

void highlight_nd_fd(int s, int t, std::string str, Hist& mHist,
                     QList<QGraphicsItem*>& mGra_Itms_ptrs,
                     std::vector<int>& mTextNoChanged,
                     std::vector<QGraphicsSimpleTextItem *>& mTexts_ptrs,
                     GraphicsScene* scene)
{
    QPen pen(Qt::green);
    QLine line(mHist.getQP_Vs().at(s),
               mHist.getQP_Vs().at(t));
    mGra_Itms_ptrs.append(scene->addLine(line, pen));
    mTextNoChanged.push_back(t);
    mTexts_ptrs.at(t)->setText(str.data());
}

void GraphicsScene::highlight_nd(int s, int t)
{
    auto str = "nd(" + std::to_string(s) +
            ", " + std::to_string(t) + ")";
    highlight_nd_fd(
                s, mRScheme.tables().at(s).find_nd(t)->first,
                str, mHist, mGra_Itms_ptrs, mTextNoChanged,
                mTexts_ptrs, this
                );
}

void GraphicsScene::highlight_fd(int s, int t)
{
    auto str = "fd(" + std::to_string(s) +
            ", " + std::to_string(t) + ")";
    highlight_nd_fd(
                s, mRScheme.tables().at(s).find_fd(t)->first,
                str, mHist, mGra_Itms_ptrs, mTextNoChanged,
                mTexts_ptrs, this
                );
}

using t_i_s = std::tuple<int, std::string>;
void GraphicsScene::draw_rout(std::list<t_i_s> rout_v_ptrs)
{
    this->reset_rout();
    QPen pen(Qt::red);
    auto [left_v, info_str] = rout_v_ptrs.front();
    rout_v_ptrs.pop_front();
    for (const auto &[right_v, info_str] : rout_v_ptrs) {
        QLine line(mHist.getQP_Vs().at(left_v),
                   mHist.getQP_Vs().at(right_v));
        mRout_Gra_Itms_ptrs.append(this->addLine(line, pen));

        QPoint p = mHist.getQP_Vs().at(right_v);
        mRout_Gra_Itms_ptrs.append(
                    this->addEllipse(
                        p.rx()-mImage_scale/2, p.ry()-mImage_scale/2,
                        mImage_scale, mImage_scale,
                        pen
                        )
                    );
        left_v = right_v;
        auto text_itm_ptr = this->addText(info_str.data());
        text_itm_ptr->setPos(line.center());
        mRout_Gra_Itms_ptrs.append(text_itm_ptr);
    }
}

void GraphicsScene::draw_only_rout(std::list<t_i_s> rout_v_ptrs)
{
    this->reset_rout();
    QList<QGraphicsItem*> temp;
    std::vector<QPoint> tempQPs;
    std::vector<std::string> tempStrs;
    std::vector<int> tempIDs;
    QPen pen(Qt::red);
    for (const auto &[v_id, info_str] : rout_v_ptrs) {
        tempQPs.push_back(mHist.getQP_Vs().at(v_id));
        tempStrs.push_back(info_str);
        tempIDs.push_back(v_id);
    }
    tempStrs.erase(tempStrs.begin());
    auto maxX = std::max_element(tempQPs.begin(), tempQPs.end(),
            [&](QPoint a, QPoint b){return a.x() < b.x();})
        ->x();
    auto maxY = std::max_element(tempQPs.begin(), tempQPs.end(),
            [&](QPoint a, QPoint b){return a.y() < b.y();})
        ->y();
    for (auto & qp : tempQPs) {
        qp.rx() = (qp.x() * 200) /  (maxX / 2);
        qp.ry() = (qp.y() * 200) /  (maxY / 2);
    }
    for (int var = 0; var < rout_v_ptrs.size()-1; ++var) {
        QLine line(tempQPs.at(var),
                   tempQPs.at(var + 1));
        temp.append(this->addLine(line, pen));
        auto text_itm_ptr = this->addText(tempStrs.at(var).data());
        text_itm_ptr->setPos(line.center());
        temp.append(text_itm_ptr);
    }
    for (size_t var = 0; var < rout_v_ptrs.size(); ++var)
    {
        auto text_ptr = this->addSimpleText(std::to_string(
                                            tempIDs.at(var)).data());

        text_ptr->setPos(tempQPs.at(var));
        temp.append(text_ptr);
    }
    remove_hist();
    mRout_Gra_Itms_ptrs = temp;
}

void GraphicsScene::reset_rout()
{
    for (auto item : mRout_Gra_Itms_ptrs) {
        this->removeItem(item);
    }
    mRout_Gra_Itms_ptrs.clear();
}

Rout_scheme GraphicsScene::rScheme() const
{
    return mRScheme;
}

void GraphicsScene::draw_opt_path(int s, int t)
{
    if (mSh_path == nullptr ) {
        mSh_path = std::unique_ptr<Short_path>(
            new Short_path(mRScheme.tables()));
    }
    auto [dst, vlist] = mSh_path->distance(s,t);
    vlist.pop_front();
    int cur = s;
    int v_id;
    QPen pen(Qt::green);
    for (auto& str1 : vlist) {
        v_id = std::stoi(str1);
        QLine line(
            mHist.getQP_Vs().at(cur),
            mHist.getQP_Vs().at(v_id)
            );
        cur = v_id;
        mRout_Gra_Itms_ptrs.append(
            this->addLine(line, pen));
        QPoint p = mHist.getQP_Vs().at(v_id);
        mRout_Gra_Itms_ptrs.append(
            this->addEllipse(
                p.rx()-mImage_scale/4, p.ry()-mImage_scale/4,
                mImage_scale/2, mImage_scale/2,
                pen
                )
            );
    }
}
