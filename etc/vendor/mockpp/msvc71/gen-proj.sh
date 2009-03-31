#!/bin/sh

sed -e 's/Version 9.00/Version 8.00/g'                           <../msvc8/mockpp/mockpp.sln                           >mockpp/mockpp.sln

sed -e 's/Version="8.00"/Version="7.10"/g' -e 's/msvc8/msvc71/g' <../msvc8/mockpp/tests/mock_test.vcproj               >mockpp/tests/mock_test.vcproj
sed -e 's/Version="8.00"/Version="7.10"/g' -e 's/msvc8/msvc71/g' <../msvc8/mockpp/tests/mock_test_win32.vcproj         >mockpp/tests/mock_test_win32.vcproj
sed -e 's/Version="8.00"/Version="7.10"/g' -e 's/msvc8/msvc71/g' <../msvc8/mockpp/mockpp.vcproj                        >mockpp/mockpp.vcproj
sed -e 's/Version="8.00"/Version="7.10"/g' -e 's/msvc8/msvc71/g' <../msvc8/mockpp/examples/examples.vcproj             >mockpp/examples/examples.vcproj
sed -e 's/Version="8.00"/Version="7.10"/g' -e 's/msvc8/msvc71/g' <../msvc8/mockpp/examples/tutorial/visitmock.vcproj   >mockpp/examples/tutorial/visitmock.vcproj
sed -e 's/Version="8.00"/Version="7.10"/g' -e 's/msvc8/msvc71/g' <../msvc8/mockpp/examples/tutorial/visitmock2.vcproj  >mockpp/examples/tutorial/visitmock2.vcproj
sed -e 's/Version="8.00"/Version="7.10"/g' -e 's/msvc8/msvc71/g' <../msvc8/mockpp/examples/tutorial/chainmock2.vcproj  >mockpp/examples/tutorial/chainmock2.vcproj
sed -e 's/Version="8.00"/Version="7.10"/g' -e 's/msvc8/msvc71/g' <../msvc8/mockpp/examples/tutorial/poormock.vcproj    >mockpp/examples/tutorial/poormock.vcproj
sed -e 's/Version="8.00"/Version="7.10"/g' -e 's/msvc8/msvc71/g' <../msvc8/mockpp/examples/tutorial/poormock_se.vcproj >mockpp/examples/tutorial/poormock_se.vcproj
sed -e 's/Version="8.00"/Version="7.10"/g' -e 's/msvc8/msvc71/g' <../msvc8/mockpp/examples/tutorial/chainmock.vcproj   >mockpp/examples/tutorial/chainmock.vcproj
sed -e 's/Version="8.00"/Version="7.10"/g' -e 's/msvc8/msvc71/g' <../msvc8/mockpp/examples/tutorial/basicmock.vcproj   >mockpp/examples/tutorial/basicmock.vcproj
sed -e 's/Version="8.00"/Version="7.10"/g' -e 's/msvc8/msvc71/g' <../msvc8/mockpp/examples/tutorial/cppunit.vcproj     >mockpp/examples/tutorial/cppunit.vcproj
sed -e 's/Version="8.00"/Version="7.10"/g' -e 's/msvc8/msvc71/g' <../msvc8/mockpp/mockpp_production.vcproj             >mockpp/mockpp_production.vcproj
sed -e 's/Version="8.00"/Version="7.10"/g' -e 's/msvc8/msvc71/g' <../msvc8/mockpp/mockpp_cxxtest.vcproj                >mockpp/mockpp_cxxtest.vcproj

sed -e 's/Version="8.00"/Version="7.10"/g' -e 's/msvc8/msvc71/g' <../msvc8/generator/xml2mockpp.vcproj                >generator/xml2mockpp.vcproj
sed -e 's/Version="8.00"/Version="7.10"/g' -e 's/msvc8/msvc71/g' <../msvc8/generator/mockpp2xml.vcproj                >generator/mockpp2xml.vcproj
