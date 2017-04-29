#include <fc/real128.hpp>
#include <boost/test/unit_test.hpp>
#include <fc/log/logger.hpp>
#include <fc/io/raw.hpp>
#include <fc/crypto/hex.hpp>
#include <boost/multiprecision/cpp_int.hpp>

BOOST_AUTO_TEST_SUITE(fc)

using fc::real128;
using std::string;

BOOST_AUTO_TEST_CASE(real128_test)
{
   BOOST_CHECK_EQUAL(string(real128()), string("0."));
   BOOST_CHECK_EQUAL(string(real128(0)), string("0."));
   BOOST_CHECK_EQUAL(real128(8).to_uint64(), 8);
   BOOST_CHECK_EQUAL(real128(6789).to_uint64(), 6789);
   BOOST_CHECK_EQUAL(real128(10000).to_uint64(), 10000);
   BOOST_CHECK_EQUAL(string(real128(1)), string("1."));
   BOOST_CHECK_EQUAL(string(real128(5)), string("5."));
   BOOST_CHECK_EQUAL(string(real128(12345)), string("12345."));
   BOOST_CHECK_EQUAL(string(real128(0)), string(real128("0")));

   real128 ten(10);
   real128 two(2);
   real128 twenty(20);
   real128 pi(31415926535);
   pi /=      10000000000;

   BOOST_CHECK_EQUAL( string(ten), "10." );
   BOOST_CHECK_EQUAL( string(two), "2." );
   BOOST_CHECK_EQUAL( string(ten+two), "12." );
   BOOST_CHECK_EQUAL( string(ten-two), "8." );
   BOOST_CHECK_EQUAL( string(ten*two), "20." );
   BOOST_CHECK_EQUAL( string(ten/two), "5." );
   BOOST_CHECK_EQUAL( string(ten/two/two/two*two*two*two), "10." );
   BOOST_CHECK_EQUAL( string(ten/two/two/two*two*two*two), string(ten) );
   BOOST_CHECK_EQUAL( string(twenty/ten), string(two) );
   BOOST_CHECK_EQUAL( string(pi), "3.1415926535" );
   BOOST_CHECK_EQUAL( string(pi*10), "31.415926535" );
   BOOST_CHECK_EQUAL( string(pi*20), "62.83185307" );
   BOOST_CHECK_EQUAL( string(real128("62.83185307")/twenty), string(pi) );
   BOOST_CHECK_EQUAL( string(pi*1), "3.1415926535" );
   BOOST_CHECK_EQUAL( string(pi*0), "0." );

   BOOST_CHECK_EQUAL(real128("12345.6789").to_uint64(), 12345);
   BOOST_CHECK_EQUAL((real128("12345.6789")*10000).to_uint64(), 123456789);
   BOOST_CHECK_EQUAL(string(real128("12345.6789")), string("12345.6789"));

   BOOST_CHECK_EQUAL( real128(uint64_t(-1)).to_uint64(), uint64_t(-1) );

   wdump( (ten)(two)(twenty) );
   wdump((real128("12345.6789")) );
   wdump( (ten/3*3) );
}

using namespace boost::multiprecision;
template<size_t Size>
using UInt = number<cpp_int_backend<Size, Size, unsigned_magnitude, unchecked, void> >;
template<size_t Size>
using Int = number<cpp_int_backend<Size, Size, signed_magnitude, unchecked, void> >;
using UInt64    = UInt<64>;
using UInt256    = UInt<256>;
BOOST_AUTO_TEST_CASE( multiprecision_test ) {


    edump((sizeof(UInt<8>)));
    edump((sizeof(UInt<16>)));
    edump((sizeof(UInt<32>)));
    edump((sizeof(UInt<64>)));
    edump((sizeof(UInt<128>)));
    edump((sizeof(UInt<256>)));
    edump((sizeof(Int<8>)));
    edump((sizeof(Int<16>)));
    edump((sizeof(Int<32>)));
    edump((sizeof(Int<64>)));
    edump((sizeof(Int<128>)));
    edump((sizeof(Int<256>)));

    char tmp[sizeof(UInt256)];
    memset( tmp, 0, sizeof(tmp) );
    new (tmp) UInt256(2);
    std::cerr << fc::to_hex( tmp, sizeof(tmp) ) <<"\n";
    UInt256* pt = (UInt256*)tmp;

    UInt256 n("0x1234567890ABCDEF0123456789ABCDF0123456789ABCDEF");
    /*
    std::cerr << fc::to_hex( (char*)&n, sizeof(n) ) <<"\n";
    std::cerr << fc::to_hex( (char*)&n, 32 ) <<"\n";
    *pt = n;
    std::cerr << fc::to_hex( tmp, sizeof(tmp) ) <<"\n";
    std::cerr << pt << "\n";;
    memset( tmp+32, 0, sizeof(tmp)-32 );
    std::cerr << fc::to_hex( tmp, sizeof(tmp) ) <<"\n";
    std::cerr << pt << "\n";;
    n = *pt;
    std::cerr << fc::to_hex( (char*)&n, sizeof(n) ) <<"\n";
    std::cerr << fc::to_hex( (char*)&n, sizeof(n) ) <<"\n";

    UInt256 n2("0x1");
    std::cerr << fc::to_hex( (char*)&n2, sizeof(n2) ) <<"\n";
    UInt256 n0(0);
    std::cerr << fc::to_hex( (char*)&n0, sizeof(n2) ) <<"\n";
    */

    std::vector<unsigned char> v;
    export_bits(n, std::back_inserter(v), 8);
    std::cerr << fc::to_hex( (char*)v.data(), v.size() ) <<"\n";
    // import back again, and check for equality:
    UInt256 j;
    import_bits(j, v.begin(), v.end());
    BOOST_CHECK_EQUAL( n, j );
    wdump((fc::to_hex((const char*)v.data(),sizeof(v.size()))));


   {
      UInt64 test(1);
      idump((test));
      uint64_t native = 1;
      auto packed_native = fc::raw::pack(native);
      auto packsize = fc::raw::pack_size(test);
      idump((packsize));
      auto packed_test = fc::raw::pack(test);
      edump((packed_test)(packed_native));
      auto unpacked = fc::raw::unpack<UInt64>(packed_test);
      edump((unpacked));

      BOOST_CHECK_EQUAL( unpacked, test );
   }
   {
      UInt256 test("0x1234567890ABCDEF0123456789ABCDF");
      idump((test));
      std::cerr << std::hex << test << "\n";;
      auto packsize = fc::raw::pack_size(test);
      idump((packsize));
      auto packed_test = fc::raw::pack(test);
      edump((packed_test));
      auto unpacked = fc::raw::unpack<UInt256>(packed_test);
      std::cerr << "\n" << std::hex << unpacked;
      edump((unpacked));

      BOOST_CHECK_EQUAL( unpacked, test );
   }
}

BOOST_AUTO_TEST_SUITE_END()
