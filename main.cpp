#include <aarith/float.hpp>

#include <iostream>
#include <cmath>

using namespace aarith;

using nf_t = normalized_float<8, 23>; // NOLINT
using nf_db = normalized_float<11, 52>; // NOLINT
using nf_s = normalized_float<4, 3>; // NOLINT

void show_float(const float& a, const nf_t& nf_a)
{
    std::cout 
        << "float: "
        << a
        << "\n nfloat: "
        << to_sci_string(nf_a) 
        << "\n\n";

}

void show_float(const double& a, const nf_db& nf_a)
{
    std::cout 
        << "double: "
        << a
        << "\n nfloat: "
        << to_sci_string(nf_a) 
        << "\n\n";

}

void show_float(const float& a, const nf_s& nf_a)
{
    std::cout 
        << "float: "
        << a
        << "\n nfloat: "
        << to_sci_string(nf_a) 
        << "\n\n";

}

int main()
{

    const double two{2.};
    const double test{8.};
    auto conversion = log10(two);

    std::cout << "conversion: " << conversion << "\n"
              << "test 2^8 = 10^" << test*conversion << "\n\n";

    const float a = 0.F;
    const float b = 0.5F;
    const float c = 1.25F;
    const float d = 0.385671F;
    const float e = 0.0117647F;
    const float f = 10.F;
    nf_t nf_a(a);
    nf_t nf_b(b);
    nf_t nf_c(c);
    nf_t nf_d(d);
    nf_t nf_e(e);
    nf_t nf_f(f);

    auto na = -a;
    auto nb = -b;
    auto nc = -c;
    auto nd = -d;
    auto ne = -e;
    auto nf = -f;
    nf_t nf_na(na);
    nf_t nf_nb(nb);
    nf_t nf_nc(nc);
    nf_t nf_nd(nd);
    nf_t nf_ne(ne);
    nf_t nf_nf(nf);

    show_float(a, nf_a);
    show_float(b, nf_b);
    show_float(c, nf_c);
    show_float(d, nf_d);
    show_float(e, nf_e);
    show_float(f, nf_f);
    show_float(na, nf_na);
    show_float(nb, nf_nb);
    show_float(nc, nf_nc);
    show_float(nd, nf_nd);
    show_float(ne, nf_ne);
    show_float(nf, nf_nf);


    const double adb = 0.;
    const double bdb = 0.5;
    const double cdb = 1.25;
    const double ddb = 0.385671;
    const double edb = 0.0117647;
    const double fdb = 10.;
    nf_db nfdb_a(adb);
    nf_db nfdb_b(bdb);
    nf_db nfdb_c(cdb);
    nf_db nfdb_d(ddb);
    nf_db nfdb_e(edb);
    nf_db nfdb_f(fdb);

    auto nadb = -adb;
    auto nbdb = -bdb;
    auto ncdb = -cdb;
    auto nddb = -ddb;
    auto nedb = -edb;
    auto nfdb = -fdb;
    nf_db nfdb_na(nadb);
    nf_db nfdb_nb(nbdb);
    nf_db nfdb_nc(ncdb);
    nf_db nfdb_nd(nddb);
    nf_db nfdb_ne(nedb);
    nf_db nfdb_nf(nfdb);

    show_float(adb, nfdb_a);
    show_float(bdb, nfdb_b);
    show_float(cdb, nfdb_c);
    show_float(ddb, nfdb_d);
    show_float(edb, nfdb_e);
    show_float(fdb, nfdb_f);
    show_float(nadb, nfdb_na);
    show_float(nbdb, nfdb_nb);
    show_float(ncdb, nfdb_nc);
    show_float(nddb, nfdb_nd);
    show_float(nedb, nfdb_ne);
    show_float(nfdb, nfdb_nf);

    nf_s nfs_a(a);
    nf_s nfs_b(b);
    nf_s nfs_c(c);
    nf_s nfs_d(d);
    nf_s nfs_e(e);
    nf_s nfs_f(f);
    nf_s nfs_na(na);
    nf_s nfs_nb(nb);
    nf_s nfs_nc(nc);
    nf_s nfs_nd(nd);
    nf_s nfs_ne(ne);
    nf_s nfs_nf(nf);

    show_float(a, nfs_a);
    show_float(b, nfs_b);
    show_float(c, nfs_c);
    show_float(d, nfs_d);
    show_float(e, nfs_e);
    show_float(f, nfs_f);
    show_float(na, nfs_na);
    show_float(nb, nfs_nb);
    show_float(nc, nfs_nc);
    show_float(nd, nfs_nd);
    show_float(ne, nfs_ne);
    show_float(nf, nfs_nf);

    return 0;
}
