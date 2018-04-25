add_executable(tmm 
    main.cxx 
    util/binaryreader.cxx 
    util/tmodfile.cxx
    util/appdata.cxx
    )

set_property(TARGET tmm PROPERTY CXX_STANDARD 11)
target_link_libraries(tmm Qt5::Widgets ${OPENSSL_LIBRARIES} ${ZLIB_LIBRARIES})
