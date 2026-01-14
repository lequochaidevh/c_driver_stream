_app_completion() {
  local cur prev cmd used
  cur="${COMP_WORDS[COMP_CWORD]}"
  prev="${COMP_WORDS[COMP_CWORD-1]}"
  cmd="${COMP_WORDS[1]}"
  used=" ${COMP_WORDS[*]} "

  if (( COMP_CWORD == 1 )); then
    COMPREPLY=( $(compgen -W "build run " -- "$cur") )
    return
  fi

  case "$cmd" in
    build)
      if [[ -z "$cur" || "$cur" == --* ]]; then
        local opts=""
        if [[ "$used" != *" --debug "* ]]; then
          if ! [[ "$used" =~ (--debug|--release) ]]; then
            opts+="--debug "
          fi
        fi
        if [[ "$used" != *" --release "* ]]; then
          if ! [[ "$used" =~ (--debug|--release) ]]; then
            opts+="--release "
          fi
        fi
        COMPREPLY=( $(compgen -W "$opts" -- "$cur") )
        return
      fi
      case "$prev" in
      esac
      ;;
    run)
      if [[ -z "$cur" || "$cur" == --* ]]; then
        local opts=""
        if [[ "$used" != *" --env "* ]]; then
          opts+="--env "
        fi
        if [[ "$used" != *" --port "* ]]; then
          opts+="--port "
        fi
        COMPREPLY=( $(compgen -W "$opts" -- "$cur") )
        return
      fi
      case "$prev" in
        --env)
          COMPREPLY=( $(compgen -W "fast slow " -- "$cur") )
          return
          ;;
      esac
      ;;
  esac
}

complete -F _app_completion app
