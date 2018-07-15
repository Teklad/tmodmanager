add_executable(tmodmanager
    main.cxx
    util/binaryreader.cxx
    util/tmodfile.cxx
    util/appdata.cxx
    util/datacache.cxx
)

set_property(TARGET tmodmanager PROPERTY CXX_STANDARD 11)

include_directories(tmodmanager PRIVATE
    ${CryptoPP_INCLUDE_DIRS}
)

target_link_libraries(tmodmanager
    ${CryptoPP_LIBRARIES}
    Qt5::Widgets
    Qt5::Network
)

install(TARGETS tmodmanager DESTINATION bin)
