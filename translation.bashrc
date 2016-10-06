# ~/.bashrc: 비로그인 쉘들을 위해 bash(1)에의해 실행됩니다.
# 예제에 관해서 (bash-doc 패키지 안에 있는)/usr/share/doc/bash/examples/startup-files를 봐주세요.

# 만약 상호작용하지 않는다면, 어떤것도 건들지 않습니다.
case $- in
	*i*) ;;
		*) return;;
esac

# 명령어 히스토리에 빈 공간으로 시작하는 라인 또는 중복된 라인을 넣지않습니다.
# 좀더 많은 옵션에 대해 bash(1)을 보십시오.
# HISTCONTROL에 대한 옵션으로 ignoredup와 ignorespace등이 있습니다.
HISTCONTORL=ignoreboth

# 명령어 히스토리 파일에 덮어 쓰지 않고 추가됩니다
shopt -s histappend

# 명령어 히스토리 길이 설정에 대해서 bash(1)에 있는 HISTSIZE와 HISTFILESIZE를 보십시오.
HISTSiZE=1000
HISTFILESIZE=2000

# 각 명령어 이후에 윈도우 크기를 확인합니다. 만약 필요하다면 라인(행)과 열을 갱신합니다.
shopt -s checkwinsize

# 설정했다면 패스이름 확장 문맥에서 사용되는 '**'패턴은 모든 파일 그리고 0개 또는 그 이상의 디렉토리와 서브 디렉토리에서도 대조할 것입니다.
#shopt -s globstar

# 비텍스트 입력 파일들에 대해 less가 사용하기 좋게 만듭니다. lesspipe(1)을 보십시오.
# lesspipe는 lesspipe와 lessfile을 합쳐놓은 프로그램으로서
# lesspipe는 표준출력으로 less파일에 문맥/정보를 입력합니다.
# 이는 lesspipe는 less프로그램이 유저가 파일을 보기전에 디코딩하는 것을 기다릴 필요없게 만듭니다.
# lessfile은 lesspipe에서 less에 읽힌 이후에 필요없는 파일들을 삭제합니다.
[ -x /usr/bin/lesspipe ] && eval "$(SHELL=/bin/sh lesspipe)"

# (프롬프트 하에 사용되어지는) 당신이 사용하는 chroot를 식별하는 변수를 설정합니다.
if [ -z "${debian_chroot:-}" ] && [ -r /etc/debian_chroot ]; then
	debian_chroot=$(cat /etc/debian_chroot)
fi

# 화려한 프롬프트를 설정합니다.(우리가 우리는 색을 "원한다"는 것을 알지 않는다면 색이 없습니다.)
case "$TERM" in
	xterm-color) color_prompt=yes;;
esac

# 터미널이 성능이 된다면 색이 입혀진 색으로 입혀진 주석을 사용합니다.;
# 유저의 정신을 혼란하게 만들지 않기 위해 기본값이 해제됩니다.: 터미널 창에 초점
# 프롬프트가 아니면 며영어의 결과물입니다.
# force_color_prompt=yes

# 위의 force_color_prompt=yes 가 주석처리가 되지 않는다면,
# Ecma-48과 호환되는 색상 컬러를 갖고 있다면
# 아래의 if문에 의해  color_prompt=yes가 됩니다.
if [ -n "$force_color_prompt" ]; then
	if [ -x /usr/bin/tput ] && tput setaf 1 > &/dev/null; then
	# 우리는 색상지원을 갖고있습니다; ecma-48)ISO/IEC-6429)와 호환
	# 된다고 가정합니다. 이러한 지원 부족은 극히 드뭅니다, 그리고 이러한 경우
	# setaf 보다 setf를 지원하는 경향을 보입니다.
	color_prompt=yes
	else
	color_prompt=
	fi
fi

# 위에서 color_prompt의 if문
if [ "$color_prompt" = yes ]; then
	# 설정되어 있으면 아래와 같은 옵션을 갖고 있는 prompt가 됩니다.
	# 바로 아래는 새강이 추가된 프롬프트를 적용하는 것이며
	# 프롬프트에 색상을 추가하는 방법은 \[\033[컬러넘버m\]를 해당 프롬프트 앞에 쓰는 것입니다.
	# 간단한 몇가지 컬러코드에 대해서 적으면
	# Black       0;30     Dark Gray     1;30
	# Blue        0;34     Light Blue    1;34
	# Green       0;32     Light Green   1;32
	# Cyan        0;36     Light Cyan    1;36
	# Red         0;31     Light Red     1;31
	# Purple      0;35     Light Purple  1;35
	# Brown       0;33     Yellow        1;33
	# Light Gray  0;37     White         1;37
	PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$'
else
	# if color_prompt=no
	# 아래는 비색상 설정일 경우 표시되는 프롬프트며
	# 다음과 같이 PS1값인 제어 문자나 문자열에 따라 출력됩니다.
	# 다음은 유저이름(\u)@호스트이름(\h):현재 경로(\w)권한(\$, 루트 유저일경우 $가 #으로 덮어씌어집니다.)
	# 그외 제어 문자
	# \t: 24시간기준 시:분:초
	# \T: 12시간기준 시:분:초
	# \@: 오전/오후
	# \a: ASCII 벨 문자(07)
	# \d: "요일 달 일"(예시: Tue May 26)
	# \e: ASCII 이스케이프 문자(033)
	# \H: 호스트 전체 이름
	# \j: 현재 쉘에서 관리되는 작업의 수
	# \l: 쉘 터미널 디바이스 이름
	# \n: newline
	# \v: 배쉬 버전
	# \V: 배포 배쉬, 버전 + 패치레벨
	# \W: 현재 작업중인 디렉토리의 이름
	# \!: 히스토리 넘버
	# \#: 커맨드 넘버
	# \[: 프롬프트에서 터미널 제어 시퀸스에 사용되는 비출력 문자의 시작
	# \]: 비출력 문자의 끝
	PS1='${debian_chroot:+($debian_chroot)}\u@\h:\w\$ '
fi
#위에서 사용한 color_prompt와 force_color_prompt값을 해제합니다.
unset color_prompt force_color_prompt

# xterm일 경우 제목을 유저명@호스트명:로 설정합니다.
case "$TERM" in
xterm*|rxvt*)
	PS="\[\e]0;${debian_chroot:(debian_chroot)}\u@\h: \w\a\]$PS1"
	;;
*)
	;;
esac

# ls 명령어가 색상 지원을 할 수 있게만듭니다. 또한 간단한 명령어도 추가합니다.
# if문에서 /usr/bin/dircolors 실행여부를 확인합니다.
if [ -x /usr/bin/dircolors ]; then
	test -r ~/.dircolors && eval "$(dircolors -b ~/,dircolors)" || eval "$(dircolors -b)"
	#ls 명령어를 ls --color=auto로 변경합니다.
	alias ls='ls --color=auto'
	#alias dir='dir --color=auto'
	#alias vdir='vidr --color=auto'
	
	#grep 명령어를 grep --color=auto로 변경합니다.
	alias grep='grep --color=auto'
	#fgrep 명령어를 fgrep --color=auto로 변경합니다.
	alias frep='frep --color=auto'
	#egrep 명령어를 egrep --color=auto로 변경합니다.
	alias egrep='egrep --color=auto'
fi

# ls 명령어를 추가합니다.
# alias 명령어명='해당 명령어 내용'
# 위와 같은 내용으로 명령어를 추가합니다.
alias ll='ls -alF'
alias la='ls -A'
alias l='ls -CF'

# 장기간 동작하는 명렁어들을 위해 "alter" 명령어를 추가합니다.
# 다음과 같이 사용합니다: sleep 10; alert
alias alert='notify-send --urgency=low -i "$([ $? = 0 ] && echo terminal || echo error)" "$(history|tail -n1|sed -e '\' 's\^\s*[0-9]\+\s*//;s[;&|]\s*alert$//'\'')"'

# 명령어 정의들.
# 여기에 직접 명령어들을 추가하는 것 대신에 ~/.bash_aliases같은 분산된 파일들 속에 당신의 추가물을 집어넣기 원할지도 모른다.
# bash-doc 패키지 안에 있는 /usr/share/doc/bash-doc/examples를 보아라.
# .bashrc가 아니더라도 외부 파일에 선언된 명령어들을 읽습니다.
if [ -f ~/.bash_aliases ]; then
	. ~/.bash_aliases
fi

# (/etc/bash.bashrc 와 /etc/profile 안에 이미 사용 가능하다면 당신은 이것을 사용할 필요가 없다.) 사용 가능한 프로그래밍 가능한 마지막 특징들
if ! shopt -oq posix; then
	if [ -f /usr/share/bash-completion/bash_completion ]; then
		. /usr/share/bash-completion/bash_completion
	elif [-f /etc/bash_completion ]; then
		. /etc/bash_completion
	fi
fi
