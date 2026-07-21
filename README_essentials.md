# Pintos Development Environment (Docker)

This repository contains the source code for the Pintos Operating System projects, configured to run in an isolated Docker container. This setup ensures that the legacy 32-bit compilation toolchain and QEMU emulator work perfectly without conflicting with modern host systems or newer CPUs.

## Prerequisites
- **Docker** installed on your host machine.
- **VS Code** (or your preferred editor) installed on your host machine.
- **GitHub Desktop** (or Git CLI) for saving checkpoints.

---

## 🛠️ Daily Development Workflow

### 1. Launch the Container (On Host)
Open a terminal on your host machine, navigate to this `pintos` directory, and start the Docker container. This command links your local code to the container so you can edit locally and compile in isolation.

```bash
docker run -it --rm --name pintos-env --mount type=bind,source=$PWD,target=/home/PKUOS/pintos pkuflyingpig/pintos bash
```
*(Note: If you are not in the pintos folder when running this, replace `$PWD` with `$HOME/pintos`)*

### 2. Configure the Path (Inside Docker)
Once the container starts, you will see a root prompt (`root@<container-id>:/#`). You must run this command every time you open the container to enable the `pintos` run commands:
```bash
export PATH=/home/PKUOS/pintos/src/utils:$PATH
```

### 3. Edit Code (On Host)
Open this `pintos` folder in **VS Code** on your host machine. Any changes you save here will instantly appear inside the Docker container.
- For Project 1, you will primarily work inside `src/threads` and `src/devices`.

### 4. Compile and Test (Inside Docker)
Switch back to your Docker terminal to compile and run your code.

**To Compile:**
```bash
cd /home/PKUOS/pintos/src/threads
make
```

**To Run a Test:**
```bash
cd build
pintos -- run alarm-multiple
```
*(Tip: If the terminal hangs after printing "Execution complete", press `Ctrl + C` to get your prompt back).*

---

## 💾 Saving Your Work
Always save checkpoints of your work when you get a test to pass!
1. Open **GitHub Desktop**.
2. Review the files you changed.
3. Write a short Summary at the bottom left (e.g., "Implemented timer_sleep").
4. Click **Commit to main**.

---

## 🛑 Closing the Environment
When you are done working for the day, simply type `exit` in the Docker terminal or press `Ctrl + D`. 

Because we use the `--rm` flag, the container will instantly delete itself to keep your computer clean. All your code remains safely saved on your host machine.
