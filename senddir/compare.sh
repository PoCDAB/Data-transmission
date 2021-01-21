if diff -r -q ./original ./received > /dev/null
then
	echo "no difference"
else
	echo "difference"
fi
