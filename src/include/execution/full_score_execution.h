//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef VEND_FULL_SCORE_EXECUTION_H
#define VEND_FULL_SCORE_EXECUTION_H



class FullScoreExecution {

public:
    FullScoreExecution(std::string vend_prefix, std::string output_path) : output_path_(output_path) {
        for (int i = 0; i < PLAN_NUMS; ++i) {
            vend_paths_[i] = VendFactory::GetVendPath(vend_prefix, Vends[i]);
            graphs_[i] = std::make_shared<Graph>("", vend_paths_[i], Vends[i]);
            graphs_[i]->Init();
        }
    }

    void Execute() {
        uint32_t scores[PLAN_NUMS];
        double percent[PLAN_NUMS];
        memset(scores, 0, sizeof(uint32_t) * PLAN_NUMS);
        memset(percent, 0, sizeof(double) * PLAN_NUMS);

        std::ofstream output(output_path_, std::ios::app | std::ios::out);
        uint64_t times=0;
        for (uint32_t i = 1; i < VERTEX_SIZE; ++i) {
            for (uint32_t j = i + 1; j <= VERTEX_SIZE; ++j) {
                ++times;
                for (int n = 0; n < PLAN_NUMS; ++n) {
                    if (graphs_[n]->GetEdge(i, j) != PairType::Uncertain)
                        ++scores[n];
                }
            }
            if (times % BATH_SIZE == 0) {
                std::cout<<"after "<<times<<" times"<<std::endl;
                for (int i = 0; i < PLAN_NUMS; ++i) {
                    percent[i] = (double) scores[i] / BATH_SIZE;
                    std::cout << "  Plan " << i << " : " << VEND_STRING[Vends[i]] << "  score : " << scores[i] << "("
                              << percent[i] << "%)" << "\n";
                    output << scores[i] << ',' << percent[i] << ',';
                }
                output << "\n";

            }
        }
        output.close();
    }

private:

    std::shared_ptr<Graph> graphs_[PLAN_NUMS];
    std::string vend_paths_[PLAN_NUMS];
    std::string output_path_;


};


#endif //VEND_FULL_SCORE_EXECUTION_H
