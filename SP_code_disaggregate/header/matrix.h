#ifndef MATRIX_H
#define MATRIX_H

#include <fstream>
#include <iostream>
#include <vector>
#include <limits>
#include <random>
#include <type_traits>

using namespace std;


////////////////////////////////////////////////////////////////////////////////
template<typename T>
class mat
{
    public:

////////////////////////////////////////////////////////////////////////////////
        //  constructors
        mat() : rows_(0), cols_(0) {}

        mat(const int rows, const int cols) : rows_(rows), cols_(cols) {
            m_ = vector<vector<T>>(rows_, vector<T>(cols_, 0));
        }

        mat(const int rows, const int cols, const T k) : rows_(rows), cols_(cols) {
            m_ = vector<vector<T>>(rows_, vector<T>(cols_, k));
        }

        mat(const int rows, const int cols, const vector<T> vals) : rows_(rows), cols_(cols) {
            m_ = vector<vector<T>>(rows_, vector<T>(cols_, 0));
            set(vals);
        }


////////////////////////////////////////////////////////////////////////////////
        //  access
        T& at(const int id) {
            const int row = floor(id / cols());
            const int col = id - row * cols();
            return at(row, col);
        }

        const T& at(const int id) const {
            const int row = floor(id / (cols()-1));
            const int col = id - row * cols();
            return at(row, col);
        }

        T& at(const int r, const int c) {
            return m_[r][c];
        }

        const T& at(const int r, const int c) const {
            return m_[r][c];
        }

        vector<T>& operator [] (const int r) {
            return m_[r];
        }

        const vector<T>& operator [] (const int r) const {
            return m_[r];
        }

		/*
		const mat<T> +(const mat<T>& m1, const mat<T>& m2) const {
		assert(m1.rows() == m2.rows());
		assert(m1.cols() == m2.cols());
		mat<T> res(m1.rows(), m1.cols());

		return ;
		}
		
		*/


////////////////////////////////////////////////////////////////////////////////
        //  getter
        int rows() const { return m_.size(); }
        int cols() const { return rows() > 0 ? m_[0].size() : 0; }
        size_t size() const { return rows() * cols(); }

        T colSum(
            const size_t row, 
            const size_t col1, 
            const size_t col2) const
        {
            T sum = 0;

            for (int col = col1; col < col2 && col < cols(); col++) 
                sum += at(row, col);

            return sum;
        }


////////////////////////////////////////////////////////////////////////////////
        //  setter
        void set(const T k)
        {
            for (auto& row : m_) {
                for (auto& val : row)
                        val = k;
            }
        }

        void set(const vector<T>& vals)
        {
            if (vals.size() != size()) {
                cerr << "warning | mat::set | wrong number of elements" << endl;
                return;
            }

            for (int i = 0; i < (int)size(); i++)
                at(i) = vals[i];
        }

        mat<T>& operator << (const vector<T>& vals) {
            set(vals);
        }


////////////////////////////////////////////////////////////////////////////////
        //  static
        static mat<T> rnd(
            const int rows, 
            const int cols, 
            const T min,
            const T max)
        {
            static random_device rd;     
            static mt19937 rng(rd());   

            mat m(rows, cols);
            
            
            // commented out due to bug(?) in old gcc version
            
//            if(is_integral<T>::value) 
//            {
//                uniform_int_distribution<T> dist(min, max);
//
//                for (int r = 0; r < rows; r++) {
//                    for (int c = 0; c < cols; c++)
//                        m.at(r, c) = dist(rng);
//                }
//            } 
//            else 
//            {
                uniform_real_distribution<T> dist(min, max);

                for (int r = 0; r < rows; r++) {
                    for (int c = 0; c < cols; c++)
                        m.at(r, c) = dist(rng);
                }
//            }

            return m;
        }


////////////////////////////////////////////////////////////////////////////////
        friend ostream& operator << (ostream& out, const mat& m)
        {
            out << "[";

            for (int r = 0; r < m.rows(); r++)
            {
                out << "[";

                for (int c = 0; c < m.cols(); c++)
                {
                    out << m.at(r, c);

                    if (c < m.cols() - 1)
                        out << ",";
                }

                out << "]";

                if (r < m.rows() - 1)
                    out << "," << endl << " ";
            }

            out << "]";

            return out;
        }


////////////////////////////////////////////////////////////////////////////////
        friend ofstream& operator << (ofstream& out, const mat& m)
        {
            out << m.rows() << " " << m.cols() << endl;

            for (int r = 0; r < m.rows(); r++)
            {
                for (int c = 0; c < m.cols(); c++) {
                    out << m.at(r, c) << " ";
                }

                out << endl;
            }

            return out;
        }


////////////////////////////////////////////////////////////////////////////////
        friend ifstream& operator >> (ifstream& in, mat& m)
        {
            int rows, cols;
            in >> rows >> cols;

            m = mat(rows, cols);

            for (int r = 0; r < m.rows(); r++)
            {
                for (int c = 0; c < m.cols(); c++) {
                    in >> m.m_[r][c];
                }
            }

            return in;
        }

    private:

////////////////////////////////////////////////////////////////////////////////
        //  memory and attributes
        vector<vector<T>> m_;
        int rows_;
        int cols_;
};


////////////////////////////////////////////////////////////////////////////////
//  typedefs
typedef mat<int> mati;
typedef mat<float> matf;
typedef mat<double> matd;


#endif // MATRIX_H
