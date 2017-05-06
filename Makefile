.PHONY: clean All

All:
	@echo "----------Building project:[ Metric - Release ]----------"
	@cd "Metric" && "$(MAKE)" -f  "Metric.mk"
clean:
	@echo "----------Cleaning project:[ Metric - Release ]----------"
	@cd "Metric" && "$(MAKE)" -f  "Metric.mk" clean
