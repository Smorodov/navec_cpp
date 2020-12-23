#include "navec.h"
//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void navec::loadModel(std::string filename, std::string words_filename)
{
    std::cout << "---------------------------" << std::endl;
    std::cout << "Loding NAVEC model file : " << filename << std::endl;
    std::cout << "---------------------------" << std::endl;
    npz_map = cnpy::npz_load(filename);
    // indexes
    indexes = npz_map["indexes"];
    std::cout << "indexes" << std::endl;
    std::cout << "indexes shape=" << "[" << indexes.shape[0] << "," << indexes.shape[1] << "]" << std::endl;
    std::cout << "-----------------" << std::endl;
        
    codes = npz_map["codes"];
    std::cout << "codes" << std::endl;
    std::cout << "codes shape=" << "[" << codes.shape[0] << "," << codes.shape[1] << "," << codes.shape[2] << "]" << std::endl;
    
    vectors = indexes.shape[0];
    qdim = codes.shape[0];
    dim = qdim * codes.shape[2];
    centroids = codes.shape[1];
    COUT_VAR(vectors);
    COUT_VAR(qdim);
    COUT_VAR(dim);
    COUT_VAR(centroids);
    precompute();

    std::ifstream file(words_filename);
    if (file.is_open())
    {
        std::string line;
        size_t i = 0;
        while (std::getline(file, line))
        {
            // using printf() in all tests for consistency
            //printf("%s", line.c_str());
            words_map[line] = i;
            ++i;
        }
        file.close();
    }

}

//--------------------------------------------------------------------------
// A = [ni x nk] B=[nk x nj]
// dst [ni x nj] 
//--------------------------------------------------------------------------
void matMul(int ni,int nj, int nk, float* A, float* B, float* dst)
{
    int i, j, k;
    bool needBuffer = false;
    if (dst == A || dst == B) { needBuffer = true; }
    float* tmp = nullptr;
    if (needBuffer)
    {
        tmp = new float[ni * nj];
    }
    else
    {
        tmp = dst;
    }

    for (i = 0; i < ni; i++)
    {
        for (j = 0; j < nj; j++)
        {
            tmp[i * nj + j] = 0;
            for (k = 0; k < nk; k++)
            {
                tmp[i * nj + j] += A[i * nk + k] * B[k * nj + j];
            }
        }
    }
    if (needBuffer)
    {
        memcpy(dst, tmp, ni * nj * sizeof(float));
        delete[] tmp;
    }
}

void navec::precompute(void)
{
    std::vector<size_t> sz(2);
    sz[0] = static_cast<size_t>(qdim);
    sz[1] = static_cast<size_t>(centroids);
    cnpy::NpyArray tmp;
    tmp = cnpy::NpyArray(sz, sizeof(float), false);
    for (int i = 0; i < qdim; ++i)
    {
        for (int j = 0; j < centroids; ++j)
        {
            float n = 0;
            float v = 0;
            for (int k = 0; k < codes.shape[2]; ++k)
            {
                // 100 256 3 
                v= codes.data<float>()[i * centroids * codes.shape[2] + j * codes.shape[2] + k];
                n += v * v;
            }
            tmp.data<float>()[i * centroids + j]=n;// qdim x centroids
        }
    }
    sz.resize(1);
    sz[0] = static_cast<size_t>(vectors);
    norm = cnpy::NpyArray(sz, sizeof(float), false);

    for (int i = 0; i < vectors; ++i)
    {
        float s = 0;
        for (int j = 0; j < qdim; ++j)
        {            
            size_t ind = indexes.data<unsigned char>()[i*qdim+j];            
            s+= tmp.data<float>()[j * centroids + ind]; // qdim x centroids
        }
        s = sqrt(s);
        norm.data<float>()[i] = s;
    }

    sz.resize(3);
    sz[0] = qdim;
    sz[1] = centroids;
    sz[2] = centroids;
    ab = cnpy::NpyArray(sz, sizeof(float), false);

    //dst[nixnj] = A[nixnk] B[nkxnj]
    int ni = centroids;
    int nj = centroids;
    int nk = codes.shape[2];
    float* B = new float[centroids * codes.shape[2]];

    for (int i = 0; i < qdim; ++i)
    {

        float* A = codes.data<float>()+(i * centroids * codes.shape[2]);
        for (int j = 0; j < nj; j++)
        {
            for (int k = 0; k < nk; k++)
            {
                B[nj * k + j] = A[k + j*nk];
            }
        }
        float* dst=ab.data<float>() + (i * centroids * centroids);
        matMul(ni,nj,nk,A,B,dst);        
    }
    delete[] B;
}
//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
float navec::similarity(std::string a_str, std::string b_str)
{
    // -------------
    // a = 478417;  // чашка
    // b = 216388;  // миска
    // sim = 0.6472517 // ожидаемый результат
    // -------------
    size_t a = words_map[a_str];
    size_t b = words_map[b_str];
    float a_norm = norm.data<float>()[a];
    float b_norm = norm.data<float>()[b];
    unsigned char* a_index = indexes.data<unsigned char>() + a * qdim;
    unsigned char* b_index = indexes.data<unsigned char>() + b * qdim;

    float ab_scalar = 0;
    for (int i = 0; i < qdim; ++i)
    {
        ab_scalar += ab.data<float>()[i * centroids * centroids + (int(a_index[i])) * centroids + (int(b_index[i]))];
    }
    return ab_scalar / a_norm / b_norm;
}
//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
navec::navec()
{
    vectors = 0;
    dim = 0;
    qdim = 0;
    centroids = 0;
}
navec::~navec()
{
    vectors=0;
    dim = 0;
    qdim = 0;
    centroids = 0;
}
