FROM archlinux:latest

RUN pacman -Sy --noconfirm && \
  pacman -S --noconfirm base-devel && \
  pacman -S --noconfirm gcc &&\
  pacman -S --noconfirm cmake &&\
  pacman -S --noconfirm ncurses &&\
  pacman -S --noconfirm vim 

WORKDIR /root/prompt-invaders

COPY . /root/prompt-invaders

RUN mkdir -p build && \
  cd build && \
  cmake .. && \
  make

CMD [ "./build/prompt_invaders" ]