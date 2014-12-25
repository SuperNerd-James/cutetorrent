SET BOOST_ROOT=E:\src\CuteTorrent\trunk\CuteTorrent\ThirdParties\boost_1_55_0\
bjam -j4 msvc-10.0 link=static runtime-link=shared boost=source dht=on geoip=static variant=debug i2p=on address-model=64
