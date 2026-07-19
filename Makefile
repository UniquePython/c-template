debug:
	cmake -B build/debug -DCMAKE_BUILD_TYPE=Debug
	cmake --build build/debug

dev:
	cmake -B build/dev -DCMAKE_BUILD_TYPE=Dev
	cmake --build build/dev

release:
	cmake -B build/release -DCMAKE_BUILD_TYPE=Release
	cmake --build build/release

RED := \033[31m
BOLD := \033[1m
GREEN := \033[32m
RESET := \033[0m

check-not-implemented:
	@if grep -rn --color=always 'LOG_NOT_IMPLEMENTED(' src/ include/ --exclude=template_log.h; then \
		printf "$(BOLD)$(RED)error:$(RESET) LOG_NOT_IMPLEMENTED(...) calls found above\n"; \
		exit 1; \
	else \
		printf "$(BOLD)$(GREEN)ok:$(RESET) no LOG_NOT_IMPLEMENTED(...) calls found\n"; \
	fi

clean:
	rm -rf build bin