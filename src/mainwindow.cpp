#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mScenePtr= new GraphicsScene(this);
    ui->graphicsView->setScene(mScenePtr);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selVertButton_clicked()
{
    mScenePtr->highlight_v(
                ui->vertNoSpinBox->value()
                );
}


void MainWindow::on_actionreset_triggered()
{
    mScenePtr->reset_highlight();
}


void MainWindow::on_actiongenerate_new_histogramm_triggered()
{
    mScenePtr->remove_hist();
    auto mHist = Hist(ui->size_spinBox->value());
    mScenePtr->set_hist(mHist);    
}

void MainWindow::on_nd_pushButton_clicked()
{
    mScenePtr->highlight_nd(
                ui->spinBox_nd_s->value(),
                ui->spinBox_nd_t->value()
                );
}


void MainWindow::on_fd_pushButton_clicked()
{
    mScenePtr->highlight_fd(
                ui->spinBox_nd_s->value(),
                ui->spinBox_nd_t->value()
                );
}


void MainWindow::on_toolButton_zoom_plus_clicked()
{
    ui->graphicsView->zoomIn();
}


void MainWindow::on_toolButton_zoom_minus_clicked()
{
    ui->graphicsView->zoomOut();
}


void MainWindow::on_actionreset_rout_triggered()
{
    mScenePtr->reset_rout();
}


void MainWindow::on_rout_pushButton_clicked()
{
    auto rout_v_ptrs = mScenePtr->rScheme().rout(ui->spinBox_nd_s->value(),
               ui->spinBox_nd_t->value());
    mScenePtr->draw_rout(rout_v_ptrs);
}

std::vector<std::ofstream> open_streams()
{
    std::vector<std::ofstream> streams(3);
    streams.at(0).open("data_hists_verts.csv"); // std::ios::app);
    streams.at(1).open("data_routs.csv"); //, std::ios::app);
    streams.at(2).open("data_rout_steps.csv"); //, std::ios::app);
    return streams;
}

void write_streams_headers(std::vector<std::ofstream>& streams)
{
    streams.at(0)
            << "hist_no,hist_size,entries_no,entries_max_len,hist_vs"
            << std::endl;
    streams.at(1)
            << "hist_no,rout_no,rout_from,rout_to,"
            << "dist_t,rout_to_t"
            << std::endl;
    streams.at(2)
            << "hist_no,rout_no,step_no,v_id,dec_id,ngbr_count,time"
            << std::endl;
}

void write_rout_steps(Rout_scheme& scheme, int s, int t, int hist_var,
         int rout_var, std::vector<std::ofstream>& streams)
{
    try {
        for (auto & [data_rout_step, time_d] :
            scheme.rout_data_string(s,t)) {
            std::vector<int> nums;
            nums.insert(nums.begin(),{hist_var, rout_var});
            nums.insert(nums.end(), data_rout_step.begin(),
                       data_rout_step.end());
            std::vector<std::string> data_line;
            std::for_each(nums.begin(), nums.end(),

                          [&data_line](int num){
                data_line.push_back(std::to_string(num));
            });
            std::stringstream sstr;
            sstr << time_d;
            data_line.push_back(sstr.str());
            streams.at(2)
                    << boost::join(data_line, ",")
                    << std::endl;
        }
    } catch (std::logic_error& e) {
        std::string str = e.what();
        str += "in Hist" + std::to_string(hist_var);
        throw std::logic_error(str);
    }
}

void write_rout_data(int s, int t, int rout_var, Short_path& sp, int hist_var,
         std::vector<std::ofstream>& streams)
{
    auto [dist_t, rout_to_t]
            = sp.distance(s,t);
    std::vector<int> nums;
    nums.insert(nums.begin(),{hist_var, rout_var, s, t});
    std::vector<std::string> data_line;
    std::for_each(nums.begin(), nums.end(),
                  [&data_line](int num)
                    { data_line.push_back(
                        std::to_string(num));
                    });
    data_line.push_back(std::to_string(dist_t));
    data_line.push_back(
                '"' + boost::join(rout_to_t, ",") + '"'
                );
    streams.at(1) << boost::join(data_line, ",")
                    << std::endl;
}

void write_hist_data(Rout_scheme scheme, Hist& hist,
    int hist_var, int hists_size,
         std::vector<std::ofstream>& streams)
{
    std::vector<std::string> data_line{
        std::to_string(hist_var), std::to_string(hists_size)};
    auto table_d = scheme.tables_to_csv_data();
    data_line.insert(data_line.end(),
                     table_d.begin(),table_d.end());
    data_line.push_back('"' +
                        boost::join(hist.verts_to_strings(), ",")
                        + '"');
    auto str = boost::join(data_line, ",");
    streams.at(0) << boost::join(data_line,",")
                  << std::endl;
}

void gen_Data()
{
    auto streams = open_streams();
    write_streams_headers(streams);
    int amount_hists = 100;
    int rout_no = 100;
    std::random_device rd;
    std::vector hist_sizes{2499, 4999, 7499, 9999, 12499, 14999, 17499, 19999};
    for (auto hist_sizes_it = hist_sizes.begin();
         hist_sizes_it != hist_sizes.end();
         std::advance(hist_sizes_it, 1)) {
        std::cout << "fertig groesse" << *(hist_sizes_it) << std::endl;
        int hists_size = *hist_sizes_it;
        std::uniform_int_distribution<int> v_id_distr(0,hists_size * 2 + 1);
        auto gen_s_t_rand =
            [&rd, &v_id_distr](){
                int s,t;
                s = t = v_id_distr(rd);
                while (s == t) {
                    t = v_id_distr(rd);
                    }
                return std::pair(s,t);
        };
        for (int cur_hist_size_hist_var = 0;
             cur_hist_size_hist_var < amount_hists ;
             ++cur_hist_size_hist_var) {
            int hist_var = amount_hists *
                    std::distance(hist_sizes.begin(),hist_sizes_it) +
                    cur_hist_size_hist_var;
            Hist hist(hists_size);
            Rout_scheme scheme(hist);
            write_hist_data(scheme, hist, hist_var, hists_size, streams);
            Short_path sp(scheme.tables());
            for (int rout_var = 0; rout_var < rout_no; ++rout_var) {
                auto [s,t] = gen_s_t_rand();
                write_rout_data(s, t,rout_var, sp, hist_var,
                         streams);
                write_rout_steps( scheme, s, t, hist_var,
                         rout_var, streams);
            }
        }
    }
    std::for_each(streams.begin(), streams.end(),
                  [&](std::ofstream & stream){stream.close();});
}

void MainWindow::on_actiongen_Data_triggered()
{
    gen_Data();
}

void MainWindow::on_importButton_clicked()
{
    std::ifstream hist_data;
    std::string line;
    hist_data.open("data_hists_verts.csv");
    // header line
    int hist_no = 1 + ui->size_spinBox->value();
    for (int line_no = 0; line_no <= hist_no; ++line_no) {
        std::getline(hist_data, line);
    }
    std::vector<std::string> split_res;
    boost::split(split_res, line, boost::is_any_of("\""));
    boost::split(split_res, split_res.at(1), boost::is_any_of(","));
    mScenePtr->remove_hist();
    mScenePtr->set_hist(Hist(split_res));
}


void MainWindow::on_actionoptimal_route_triggered()
{
    mScenePtr->draw_opt_path(
        ui->spinBox_nd_s->value(),
        ui->spinBox_nd_t->value());
}


void MainWindow::on_importRoutePushButton_clicked()
{
    auto rout_v_ptrs = mScenePtr->rScheme().rout(ui->spinBox_nd_s->value(),
                                                 ui->spinBox_nd_t->value());
    mScenePtr->draw_only_rout(rout_v_ptrs);
}

