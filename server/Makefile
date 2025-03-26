APP_NAME = c2server
BUILD_DIR = bin

.PHONY: all build run clean

all: build

build:
	@echo "[+] Building $(APP_NAME)..."
	@mkdir -p $(BUILD_DIR)

	@echo " [+] macOS ARM64..."
	GOOS=darwin GOARCH=arm64 go build -o $(BUILD_DIR)/$(APP_NAME)-mac-arm64 main.go

	@echo "[+] Linux x64..."
	GOOS=linux GOARCH=amd64 go build -o $(BUILD_DIR)/$(APP_NAME)-linux main.go

	@echo "[+] Windows x64..."
	GOOS=windows GOARCH=amd64 go build -o $(BUILD_DIR)/$(APP_NAME).exe main.go

	@echo "[+] Cross-platform builds complete."

run:
	@echo "[*] Running $(APP_NAME)..."
	go run main.go

clean:
	@echo "[*] Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR)
