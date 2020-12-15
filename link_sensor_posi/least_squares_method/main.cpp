#include <iostream>
#include <string>
#include <vector>
#include <Eigen/Core>
#include <Eigen/LU>
#include <Eigen/SVD>

class Csv{
    std::string csv_file;
public:
    //コンストラクタ
    Csv(std::string);
    // CSVデータ取得
    bool getCsv(std::vector<std::vector<std::string>>&, const char delim = ',');
};

int main(){
    
    int param_count = 3 ;
    std::cin >> param_count;
    Eigen::MatrixXd A1(7,4);
    Eigen::MatrixXd A2(7,4);
    Eigen::MatrixXd A(param_count,4);

    double s1_1 = 0.4;
    double s1_2 = 0.3;
    double s1_3 = 0.0;
    double s1_4 = 0.5;
    double s1_5 = 0.20;
    double s1_6 = 0.1;
    double s1_7 = 0.45;
    

    double s2_1 =  -1.8;
    double s2_2 =  -1.85;
    double s2_3 =  -2.5;
    double s2_4 =  -1.35;
    double s2_5 =  -1.60; 
    double s2_6 =  -2.10;
    double s2_7 =  -1.4;
    
    
    double ar_x_1 = 5.192;
    double ar_x_2 = 4.946;
    double ar_x_3 = 2.795; 
    double ar_x_4 = 6.447;
    double ar_x_5 = 5.565;
    double ar_x_6 = 3.886;
    double ar_x_7 = 6.350;
    
    /*
    double ar_x_1 = 5.236;
    double ar_x_2 = 4.994;
    double ar_x_3 = 2.797;
    double ar_x_4 = 6.445;
    double ar_x_5 = 5.59;
    double ar_x_6 = 3.92;
    double ar_x_7 = 6.348;
    */
    
    double ar_y_1 =  0.662;
    //double ar_y_2 =  0.062;
    double ar_y_2 =  0.065;
    //double ar_y_3 = -0.507;
    double ar_y_3 = -0.524;
    //double ar_y_4 = 1.913;
    double ar_y_4 = 1.953;
    //double ar_y_5 = -0.475;
    double ar_y_5 = -0.495;
    //double ar_y_6 = -0.825;
    double ar_y_6 = -0.925;
    double ar_y_7 =  1.460;
    
    /* 
    double ar_y_1 = 0.668;
    double ar_y_2 = 0.061;
    double ar_y_3 = -0.508;
    double ar_y_4 = 1.91;
    double ar_y_5 = -0.477;
    double ar_y_6 = -0.841;
    double ar_y_7 = 1.457;
    */
    
    A1 << std::cos(s1_1),std::cos(s1_1+s2_1),1,0,
          std::cos(s1_2),std::cos(s1_2+s2_2),1,0,
          std::cos(s1_3),std::cos(s1_3+s2_3),1,0,
          std::cos(s1_4),std::cos(s1_4+s2_4),1,0,
          std::cos(s1_5),std::cos(s1_5+s2_5),1,0,
          std::cos(s1_6),std::cos(s1_6+s2_6),1,0,
          std::cos(s1_7),std::cos(s1_7+s2_7),1,0;

    A2 << std::sin(s1_1),std::sin(s1_1+s2_1),0,1,
          std::sin(s1_2),std::sin(s1_2+s2_2),0,1,
          std::sin(s1_3),std::sin(s1_3+s2_3),0,1,
          std::sin(s1_4),std::sin(s1_4+s2_4),0,1,
          std::sin(s1_5),std::sin(s1_5+s2_5),0,1,
          std::sin(s1_6),std::sin(s1_6+s2_6),0,1,
          std::sin(s1_7),std::sin(s1_7+s2_7),0,1;
    
    
    A  << std::cos(s1_1),std::cos(s1_1+s2_1),1,0,
          std::cos(s1_2),std::cos(s1_2+s2_2),1,0,
          std::cos(s1_3),std::cos(s1_3+s2_3),1,0,
          std::cos(s1_4),std::cos(s1_4+s2_4),1,0,
          std::cos(s1_5),std::cos(s1_5+s2_5),1,0,
          std::cos(s1_6),std::cos(s1_6+s2_6),1,0,
          std::cos(s1_7),std::cos(s1_7+s2_7),1,0,
          std::sin(s1_1),std::sin(s1_1+s2_1),0,1,
          std::sin(s1_2),std::sin(s1_2+s2_2),0,1,
          std::sin(s1_3),std::sin(s1_3+s2_3),0,1,
          std::sin(s1_4),std::sin(s1_4+s2_4),0,1,
          std::sin(s1_5),std::sin(s1_5+s2_5),0,1,
          std::sin(s1_6),std::sin(s1_6+s2_6),0,1,
          std::sin(s1_7),std::sin(s1_7+s2_7),0,1;
    //std::cout<<"A1 = " << std::endl << A1 << std::endl;
    std::cout<<"A = " << std::endl << A << std::endl;



    Eigen::VectorXd b1(7);
    Eigen::VectorXd b2(7);
    Eigen::VectorXd b(param_count);
    

    b1 <<ar_x_1,ar_x_2,ar_x_3,ar_x_4,ar_x_5,ar_x_6,ar_x_7;
    b2 <<ar_y_1,ar_y_2,ar_y_3,ar_y_4,ar_y_5,ar_y_6,ar_y_7;
    b <<ar_x_1,ar_x_2,ar_x_3,ar_x_4,ar_x_5,ar_x_6,ar_x_7,
        ar_y_1,ar_y_2,ar_y_3,ar_y_4,ar_y_5,ar_y_6,ar_y_7;
    //std::cout<<"b1 = " << std::endl << b1 << std::endl;
    //std::cout<<"b2 = " << std::endl << b2 << std::endl;
    std::cout << "b = " << std::endl <<b << std::endl;


    Eigen::JacobiSVD<Eigen::MatrixXd> svd1(A1,Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::JacobiSVD<Eigen::MatrixXd> svd2(A2,Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::VectorXd x = svd1.solve(b1);
    Eigen::VectorXd z = svd2.solve(b2);
    
    Eigen::JacobiSVD<Eigen::MatrixXd> svd (A,Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::VectorXd L = svd.solve(b);

    //Eigen::VectorXd x = (A1.inverse())*b1;
    //Eigen::VectorXd z = (A2.inverse())*b2;
    //Eigen::VectorXd z = A2.fullPivLu().solve(b2);
    //Eigen::VectorXd x = A.colPivHouseholderQr().solve(b);

    std::cout<<"x = " << std::endl << x << std::endl;
    std::cout<<"y = " << std::endl << z << std::endl;

    std::cout<<"L = " << std::endl << L << std::endl;
    return 0;
}
