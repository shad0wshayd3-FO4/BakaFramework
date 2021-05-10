vcpkg_fail_port_install(ON_ARCH "arm" "arm64" ON_TARGET "osx" "uwp")

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO marzer/tomlplusplus
    REF 14682c1ece962b820925ec7c4876424651be1311
    SHA512 bfd006d358f9fbf5d95e70308b7cba051fdb313e08cb1aa529b61d34e463ab1cbf4bab938c69fe6782b03a10062e84673945010b5fd8831b1dbafb3fab40efce
    HEAD_REF master
)

vcpkg_configure_meson(
    SOURCE_PATH ${SOURCE_PATH}
    OPTIONS
        -Dgenerate_cmake_config=true
        -Dbuild_tests=false
        -Dbuild_examples=false
)

vcpkg_install_meson()
vcpkg_fixup_cmake_targets(CONFIG_PATH lib/cmake/${PORT})
vcpkg_fixup_pkgconfig()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug"
                    "${CURRENT_PACKAGES_DIR}/lib")

file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/${PORT} RENAME copyright)