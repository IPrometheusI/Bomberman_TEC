#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
requirements_file="${repo_root}/requirements.txt"

if [[ ! -f "${requirements_file}" ]]; then
	echo "requirements.txt not found at ${requirements_file}"
	exit 1
fi

detect_os() {
	local uname_out
	uname_out="$(uname -s)"
	if [[ "${uname_out}" == "Darwin" ]]; then
		echo "macos"
		return
	fi

	case "${uname_out}" in
		MINGW*|MSYS*|CYGWIN*)
			echo "windows"
			return
			;;
	esac

	if [[ -f /etc/os-release ]]; then
		. /etc/os-release
		case "${ID:-}" in
			ubuntu|debian)
				echo "debian"
				return
				;;
			fedora|rhel|centos)
				echo "fedora"
				return
				;;
			arch|manjaro)
				echo "arch"
				return
				;;
		esac
	fi

	echo "unknown"
}

map_pkg() {
	local pkg="$1"
	local os="$2"

	case "${pkg}" in
		libsdl2-dev)
			case "${os}" in
				macos|arch)
					echo "sdl2"
					;;
				fedora)
					echo "SDL2-devel"
					;;
				windows)
					echo "mingw-w64-x86_64-SDL2"
					;;
				*)
					echo "libsdl2-dev"
					;;
			esac
			;;
		SDL2)
			case "${os}" in
				debian)
					echo "libsdl2-dev"
					;;
				fedora)
					echo "SDL2-devel"
					;;
				windows)
					echo "mingw-w64-x86_64-SDL2"
					;;
				*)
					echo "sdl2"
					;;
			esac
			;;
		gcc)
			case "${os}" in
				windows)
					echo "mingw-w64-x86_64-gcc"
					;;
				*)
					echo "gcc"
					;;
			esac
			;;
		make)
			case "${os}" in
				windows)
					echo "mingw-w64-x86_64-make"
					;;
				*)
					echo "make"
					;;
			esac
			;;
		*)
			echo "${pkg}"
			;;
	esac
}

detected_os="$(detect_os)"

label_macos="macOS"
label_debian="Debian/Ubuntu"
label_fedora="Fedora/RHEL"
label_arch="Arch"
label_windows="Windows (MSYS2)"

if [[ "${detected_os}" == "macos" ]]; then
	label_macos="macOS (detected)"
elif [[ "${detected_os}" == "debian" ]]; then
	label_debian="Debian/Ubuntu (detected)"
elif [[ "${detected_os}" == "fedora" ]]; then
	label_fedora="Fedora/RHEL (detected)"
elif [[ "${detected_os}" == "arch" ]]; then
	label_arch="Arch (detected)"
elif [[ "${detected_os}" == "windows" ]]; then
	label_windows="Windows (MSYS2, detected)"
fi

echo "Select your OS (detected: ${detected_os})"
echo "1) ${label_macos}"
echo "2) ${label_debian}"
echo "3) ${label_fedora}"
echo "4) ${label_arch}"
echo "5) ${label_windows}"
echo "q) Quit"

read -r -p "Choice [default: ${detected_os}]: " choice

if [[ -z "${choice}" ]]; then
	choice="${detected_os}"
fi

case "${choice}" in
	1|macos)
		target_os="macos"
		;;
	2|debian|ubuntu)
		target_os="debian"
		;;
	3|fedora|rhel|centos)
		target_os="fedora"
		;;
	4|arch|manjaro)
		target_os="arch"
		;;
	5|windows|msys2)
		target_os="windows"
		;;
	q|Q|quit|exit)
		exit 0
		;;
	*)
		echo "Unknown selection: ${choice}"
		exit 1
		;;
esac

mapfile -t raw_pkgs < <(awk 'NF && $1 !~ /^#/' "${requirements_file}")
pkgs=()
for pkg in "${raw_pkgs[@]}"; do
	pkgs+=("$(map_pkg "${pkg}" "${target_os}")")
done

if [[ "${#pkgs[@]}" -eq 0 ]]; then
	echo "No packages found in requirements.txt"
	exit 1
fi

case "${target_os}" in
	macos)
		if ! command -v brew >/dev/null 2>&1; then
			echo "Homebrew not found. Install it from https://brew.sh/ and re-run."
			exit 1
		fi
		brew install "${pkgs[@]}"
		;;
	debian)
		sudo apt-get update
		sudo apt-get install -y "${pkgs[@]}"
		;;
	fedora)
		sudo dnf install -y "${pkgs[@]}"
		;;
	arch)
		sudo pacman -Sy --needed "${pkgs[@]}"
		;;
	windows)
		if ! command -v pacman >/dev/null 2>&1; then
			echo "MSYS2 not detected. Install MSYS2 and run this script from the MSYS2 MinGW64 shell."
			echo "https://www.msys2.org/"
			exit 1
		fi
		pacman -Sy --needed "${pkgs[@]}"
		;;
esac

echo "Dependencies installed."
