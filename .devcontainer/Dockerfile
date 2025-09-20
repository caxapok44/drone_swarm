FROM ubuntu:22.04

SHELL [ "/bin/bash", "-o", "pipefail", "-c" ]

ENV DEBIAN_FRONTEND=noninteractive

# Minimal utilities only; no build tools installed
RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    curl \
    sudo \
    locales \
 && rm -rf /var/lib/apt/lists/*

# Locale
RUN locale-gen en_US.UTF-8 && update-locale LANG=en_US.UTF-8
ENV LANG=en_US.UTF-8
ENV LC_ALL=en_US.UTF-8

# Non-root user for VS Code Remote - minimal setup
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=1000
RUN groupadd --gid ${USER_GID} ${USERNAME} \
    && useradd -s /bin/bash --uid ${USER_UID} --gid ${USER_GID} -m ${USERNAME} \
    && echo "${USERNAME} ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/${USERNAME} \
    && chmod 0440 /etc/sudoers.d/${USERNAME}

WORKDIR /workspaces/grpc_stock_market


