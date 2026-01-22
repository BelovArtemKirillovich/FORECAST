cd ../
rm -rf html_report
lcov --capture --directory . --output-file coverage.info --ignore-errors mismatch
lcov --remove coverage.info '/usr/*' '*/_deps/*' '*/tests/*' --output-file coverage_cleaned.info --ignore-errors mismatch
genhtml coverage_cleaned.info --output-directory html_report --ignore-errors mismatch
