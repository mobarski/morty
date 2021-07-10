
function io_get(dev) {
	switch (dev) {
		case 1: screen_get(); break;
	}
}

function io_set(dev) {
	switch (dev) {
		case 1: screen_set(); break;
	}
}

// ---[ SCREEN ]---------------------------------------------------------------

function screen_get() {
	fun = s_pop()
	switch (fun) {
		case 3: s_push(scr.c);                     break; // scr-get-c
		case 4: s_push(scr.i);                     break; // scr-get-i
		case 5: s_push(0);                         break; // TODO: scr-get (pixel-color)
		case 6: s_push(scr.w);                     break; // scr-get-w
		case 7: s_push(scr.h);                     break; // scr-get-h
	}
}

function screen_set() {
	fun = s_pop()
	switch (fun) {
		case 0: init_screen(scr.w, scr.h); break; // scr-init
		case 1: flip();                    break; // scr-flip
		case 2: cls(scr.c);                break; // scr-cls
		case 3: scr.c = s_pop();           break; // scr-set-c
		case 4: scr.i = s_pop();           break; // scr-set-i
		case 5: pset(scr.i++);             break; // scr-put
		case 6: scr.w = s_pop();           break; // scr-set-w
		case 7: scr.h = s_pop();           break; // scr-set-h
	}
}
