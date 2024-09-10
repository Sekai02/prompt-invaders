# **Promt Invaders**
O.S small project with some memory handling, threads, input & output.

![Title Screen](https://i.imgur.com/UlfHVtj.png)
![Game Screen](https://i.imgur.com/blfD21e.png)
![Game Over Screen](https://i.imgur.com/FfopNy4.png)

### How to run the **Promt Invaders**

The easiest and simplest way to run **Promt Invaders** is to run it with docker. To run the project using Docker, follow these steps:

1. **Build the Docker image**:
  ```sh
  docker build -t prompt-invaders .
  ```

2. **Run the Docker container**:
  ```sh
  docker run -it --rm prompt-invaders
  ```

Make sure you have Docker installed and running on your machine. If not check the [oficial docker installation docs](https://docs.docker.com/get-started/get-docker/)

### Alternative: Run the **Promt Invaders** without Docker

To run the project directly on your machine, follow these steps:

1. **Install dependencies**:
  Make sure you have `cmake` and `ncurses` installed. On a Debian-based distribution, you can install them with:
  ```sh
  sudo apt-get update
  sudo apt-get install cmake libncurses5-dev libncursesw5-dev
  ```

2. **Clone the repository**:
  ```sh
  git clone https://github.com/Sekai02/prompt-invaders.git
  cd prompt-invaders
  ```

3. **Build the project**:
  ```sh
  mkdir build
  cd build
  cmake ..
  make
  ```

4. **Run the project**:
  ```sh
  ./prompt-invaders
  ```

Make sure you have the necessary permissions to execute these commands in your terminal.