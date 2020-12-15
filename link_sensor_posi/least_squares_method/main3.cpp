#include <iostream>
#include <string>
#include <vector>

#include <fstream>
#include <sstream>

#include <Eigen/Core>
#include <Eigen/LU>
#include <Eigen/SVD>

class Csv
{
    std::string csv_file;

public:
    //constructor
    Csv(std::string);
    //CSV データ取得
    bool getCsv(std::vector<std::vector<std::string>>&, const char delim = ',');
};

Csv::Csv(std::string csv_file)
{
    this->csv_file = csv_file;
}

bool Csv::getCsv(std::vector<std::vector<std::string>>& data, const char delim)
{
    //ファイルOPEN
    std::ifstream ifs(csv_file);
    if (!ifs.is_open()) return false;

    //ファイルREAD
    std::string buf;
    while (std::getline(ifs,buf)){
        std::vector<std::string> rec;   // 一行分
        std::istringstream iss(buf);    // 文字列ストリーム
        std::string field;              // 一列分文字列

        while (getline(iss, field, delim)) rec.push_back(field);

        if(rec.size() !=0) data.push_back(rec);


    }
    return true;
}

int main()
{
    const std::string csv_file = "param2.csv";
    std::vector<std::vector<std::string>> data;
    try {
        Csv objCsv(csv_file);
        if(!objCsv.getCsv(data)){
            std::cout <<"[Error]読み込み失敗"<<std::endl;
            return 1;
        }
    }

    catch (...){
        std::cout<<"EXCEPTION"<<std::endl;
        return 1;
    }
    
    //ここから行列計算
    int param_count = 0;
    std::cout<<data[0].size()<<std::endl;
    std::cout<<data.size()<<std::endl;
    Eigen::MatrixXd A((data[0].size()-1)*2 , data.size());
    
    for(int i = 0; i<data[0].size()-1;i++){
        //実験調整////////////////////////
        std::cout<<"aaaaa"<<((180-(std::stod(data[0][i+1])))*(M_PI/180))<<std::endl;
        A(i,0)=(std::cos((180-(std::stod(data[0][i+1])))*(M_PI/180)));
        A(i,1)=(std::cos(-1*((std::stod(data[1][i+1]))-90)*(M_PI/180)));
        //////////////////////////////////
        //A(i,0)=(std::cos(std::stod(data[0][i+1])));
        //A(i,1)=(std::cos(std::stod(data[1][i+1])));
        A(i,2)=(1.0);
        A(i,3)=(0.0);    
    }

    for(int i = 0; i<data[0].size()-1;i++){
        //実験調整
        A(data[0].size()-1+i,0)=(std::sin((180-(std::stod(data[0][i+1])))*(M_PI/180)));
        A(data[0].size()-1+i,1)=(std::sin(-1*((std::stod(data[1][i+1]))-90)*(M_PI/180)));
        //////////
        //A(data[0].size()-1+i,0)=(std::sin(std::stod(data[0][i+1])));
        //A(data[0].size()-1+i,1)=(std::sin(std::stod(data[1][i+1])));
        A(data[0].size()-1+i,2)=(0.0);
        A(data[0].size()-1+i,3)=(1.0);
    }

    
    Eigen::VectorXd b((data[0].size()-1)*2);

    for(int i = 0; i<(data[0].size()-1); i++){
        b(i)=std::stod(data[2][i+1]);
    }

    for(int i = 0; i<(data[0].size()-1); i++){
        b(i+data[0].size()-1)=std::stod(data[3][i+1]);
    }
    

    Eigen::JacobiSVD<Eigen::MatrixXd> svd (A,Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::VectorXd L = svd.solve(b);

    std::cout <<"A = "<< std::endl<<A<<std::endl;
    std::cout <<"b = "<< std::endl<<b<<std::endl;
    std::cout <<"L = "<< std::endl<<L<<std::endl;
 
    return 0;
}
