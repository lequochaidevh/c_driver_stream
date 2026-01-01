export RED="\e[31m"
export GREEN="\e[32m"
export YELLOW="\e[33m"
export BLUE="\e[34m"
export MAGENTA="\e[35m"
export CYAN="\e[36m"
export WHITE="\e[37m"
export RESET="\e[0m"
export BOLD="\e[1m"
export UNDERLINE="\e[4m"

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)

echo ""

source $SCRIPT_DIR/info.sh

# export PS1='\n┌['"${BLUE}${BOLD}\w${RESET}] ${MAGENTA}${UNDERLINE}"'$(printf "%*s" $(( $(tput cols) - ${#PWD} - 18 )) "" | tr " " " " ) '"${RESET}  ${CYAN}${BOLD}$(date +%D-%T) ${RESET}"'\n│\n╰──> '
# export PS1='\n┌['"${BOLD}${GREEN}\u@ ${BLUE}\w${RESET}] ${MAGENTA}${UNDERLINE}"'$(printf "%*s" $(( $(tput cols) - ${#PWD} - 18 )) "" | tr " " " " ) '"${RESET}  ${YELLOW}( $(git_branch) ) ${RESET}"'\n│\n╰──> '


git_branch() {
    local branch
    branch=$(git rev-parse --abbrev-ref HEAD 2>/dev/null)
    if [[ -n "$branch" ]]; then
        echo "$branch"
    fi
}

export PS1_FIXED=1

set_prompt() {
    local cols=$(tput cols)
    local pwd_len=${#PWD}
    local endcmd="${YELLOW}($(git_branch)) ${CYAN}$(date +%D-%T)${RESET}"
    local endcmd_len=${#endcmd}
    local padding_len=$((cols - pwd_len - endcmd_len + 10))
    local padding=$(printf "%*s" "$padding_len" "" | tr " " " ")
    if [ ${PS1_FIXED} -ne 0 ]; then
	PS1="\n┌[${BOLD}${GREEN}\u@ ${BLUE}\w${RESET}] ${WHITE}${UNDERLINE}${padding}${RESET} ${endcmd}\n│\n╰──> "
    fi
}

PROMPT_COMMAND=set_prompt
