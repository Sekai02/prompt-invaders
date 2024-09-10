FROM alpine:latest

# Instala las dependencias necesarias
RUN apk update && \
  apk add --no-cache build-base cmake ncurses-dev vim

# Establece el directorio de trabajo
WORKDIR /root/prompt-invaders

# Copia el contenido del proyecto al contenedor
COPY . /root/prompt-invaders

# Construye el proyecto
RUN mkdir -p build && \
  cd build && \
  cmake .. && \
  make

# Comando para ejecutar la aplicación
CMD ["./build/prompt_invaders"]