add_executable(tmodmanager
    main.cxx
    util/binaryreader.cxx
    util/tmodfile.cxx
    util/appdata.cxx
	util/steamconfig.cxx
)

set_property(TARGET tmodmanager PROPERTY CXX_STANDARD 11)

include_directories(tmodmanager PRIVATE
    ${OPENSSL_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIRS}
)

target_link_libraries(tmodmanager
    ${OPENSSL_LIBRARIES}
    ${ZLIB_LIBRARIES}
	Qt5::Widgets
	Qt5::Network
)

install(TARGETS tmodmanager DESTINATION bin)
