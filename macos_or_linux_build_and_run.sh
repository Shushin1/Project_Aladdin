#!/usr/bin/env bash
# Build and run Project_Aladdin on macOS or Linux.
set -euo pipefail

cd "$(dirname "$0")"
ROOT="$(pwd)"

BUILD_DIR=build
EXE_NAME="Project_Aladdin"
SOURCES=(main.cpp shader.cpp)

OS="$(uname -s)"

pkg_exists() {
    command -v pkg-config >/dev/null 2>&1 && pkg-config --exists "$1"
}

check_dependencies() {
    MISSING=()

    if ! command -v c++ >/dev/null 2>&1; then
        MISSING+=(compiler)
    fi

    if [[ "$OS" == "Darwin" ]]; then
        if ! command -v brew >/dev/null 2>&1; then
            MISSING+=(homebrew)
        else
            brew list --versions glfw >/dev/null 2>&1 || MISSING+=(glfw)
            brew list --versions glew >/dev/null 2>&1 || MISSING+=(glew)
        fi
    else
        pkg_exists glfw3 || MISSING+=(glfw)
        pkg_exists glew || MISSING+=(glew)
    fi
}

check_dependencies

if ((${#MISSING[@]} > 0)); then
    echo "Missing dependencies: ${MISSING[*]}"

    if [[ "$OS" == "Darwin" ]]; then
        if [[ " ${MISSING[*]} " == *" homebrew "* ]]; then
            echo "Install Homebrew from https://brew.sh, then run this script again."
            exit 1
        fi
        if [[ " ${MISSING[*]} " == *" compiler "* ]]; then
            echo "Install the compiler with: xcode-select --install"
            exit 1
        fi

        read -r -p "Run 'brew install ${MISSING[*]}' now? [y/N] " INSTALL_DEPS
        case "$INSTALL_DEPS" in
            y|Y|yes|YES|Yes)
                brew install "${MISSING[@]}"
                ;;
            *)
                echo "Dependencies were not installed. Exiting."
                exit 1
                ;;
        esac

        check_dependencies
        if ((${#MISSING[@]} > 0)); then
            echo "The following dependencies are still missing: ${MISSING[*]}"
            exit 1
        fi
    else
        echo "Install them with your package manager, for example:"
        echo "  Debian/Ubuntu: sudo apt install build-essential pkg-config libglfw3-dev libglew-dev"
        echo "  Fedora:        sudo dnf install gcc-c++ pkgconf-pkg-config glfw-devel glew-devel"
        echo "  Arch:          sudo pacman -S base-devel pkgconf glfw glew"
        exit 1
    fi
fi

# Compiler and linker flags
CXXFLAGS=(-std=c++14 -g -Wall -Wno-deprecated -Wno-register -I"$ROOT")

if [[ "$OS" == "Darwin" ]]; then
    BREW_PREFIX="$(brew --prefix)"
    # Use the bundled headers in dependente/, link against Homebrew's libraries
    LDFLAGS=(-L"$BREW_PREFIX/lib" -lglfw -lGLEW -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo)
else
    LDFLAGS=($(pkg-config --libs glfw3 glew) -lGL)
fi

mkdir -p "$BUILD_DIR"

echo "Compiling ${SOURCES[*]}..."
c++ "${CXXFLAGS[@]}" "${SOURCES[@]}" -o "$BUILD_DIR/$EXE_NAME" "${LDFLAGS[@]}"

# The program loads its shaders by relative path, so run it from the project root
if [[ "$OS" == "Linux" && "${XDG_SESSION_TYPE:-}" == "wayland" ]]; then
    # GLEW is usually built for X11 (GLX); run through XWayland so glewInit() works
    unset WAYLAND_DISPLAY
    export XDG_SESSION_TYPE=x11
fi

echo "Running $EXE_NAME..."
"./$BUILD_DIR/$EXE_NAME"
