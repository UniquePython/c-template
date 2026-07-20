debug:
	cmake -B build/debug -DCMAKE_BUILD_TYPE=debug
	cmake --build build/debug

dev:
	cmake -B build/dev -DCMAKE_BUILD_TYPE=dev
	cmake --build build/dev

release:
	cmake -B build/release -DCMAKE_BUILD_TYPE=release
	cmake --build build/release

RED := \033[31m
BOLD := \033[1m
GREEN := \033[32m
RESET := \033[0m

check-release-ready:
	@if grep -rn --color=always 'TEMPLATE_LOG_NOT_IMPLEMENTED(' src/ include/ --exclude=template_log.h; then \
		printf "$(BOLD)$(RED)error:$(RESET) TEMPLATE_LOG_NOT_IMPLEMENTED(...) calls found above\n"; \
		exit 1; \
	else \
		printf "$(BOLD)$(GREEN)ok:$(RESET) no TEMPLATE_LOG_NOT_IMPLEMENTED(...) calls found\n"; \
	fi

	@if grep -rn --color=always 'TEMPLATE_ASSERT_DEBUG(' src/ include/ --exclude=template_assert.h; then \
		printf "$(BOLD)$(RED)error:$(RESET) TEMPLATE_ASSERT_DEBUG(...) calls found above\n"; \
		exit 1; \
	else \
		printf "$(BOLD)$(GREEN)ok:$(RESET) no TEMPLATE_ASSERT_DEBUG(...) calls found\n"; \
	fi

clean:
	rm -rf build bin