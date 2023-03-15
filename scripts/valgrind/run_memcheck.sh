mkdir -p memcheck

valgrind --tool=memcheck \
    --leak-check=full \
    --show-leak-kinds=all \
    --track-origins=yes \
    --verbose \
    --xml=yes \
    --xml-file=memcheck/memcheck.xml \
    --log-file=memcheck/memcheck.log \
    --error-exitcode=1 \
    ./build/Debug/examples/example_remove

    # --exit-on-first-error=yes \

ret=$?
echo ""
echo "memcheck exit code: " $ret
exit $ret