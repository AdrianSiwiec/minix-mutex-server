# usage ./multiplePush <toPush
while IFS='' read -r line || [[ -n "$line" ]]; do
    ./pushMinix.sh $line
done
