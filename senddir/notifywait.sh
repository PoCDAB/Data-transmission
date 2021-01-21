inotifywait -m ~/chris/senddir/sendfile -e create -e moved_to |
	while read dir action file; do
		echo "The file '$file' appeared in directory '$dir' via '$action'"
		cd $dir/..
		python send.py $dir/$file
		cd $dir
		mv "$file" ..
	done
