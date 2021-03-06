
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
		case 1: s_push(scr.w);                     break;
		case 2: s_push(scr.h);                     break;
		case 3: s_push(scr.pal.length/4);          break;
		case 4: s_push(scr.c);                     break;
		case 5: s_push(scr.i);                     break;
		case 6:
			ci = scr.c*4
			s_push(scr.pal[ci+0]) // R
			s_push(scr.pal[ci+1]) // G
			s_push(scr.pal[ci+2]) // B
			break
		case 13: s_push(pget()); scr.i++;          break;
		case 15: s_push(scr.touch_pos);            break; 
	}
}

function screen_set() {
	fun = s_pop()
	switch (fun) {
		case 1: scr.w = s_pop();           break; // scr-set-w
		case 2: scr.h = s_pop();           break; // scr-set-h
		case 3: break; // TODO: set palete length
		case 4: scr.c = s_pop();           break; // scr-set-c
		case 5: scr.i = s_pop();           break; // scr-set-i
		case 6:
			ci = scr.c*4
			scr.pal[ci+2] = s_pop() // B
			scr.pal[ci+1] = s_pop() // G
			scr.pal[ci+0] = s_pop() // R
			break
		case 10: n=s_pop(); fill(scr.i, n, scr.c); scr.i+=n; break;
		case 11: init_screen(scr.w, scr.h);  break; // scr-init
		case 12: flip();                     break; // scr-flip
		case 13: pset(scr.i++);              break; // scr-put
		case 14: v=s_pop(); set_draw_fun(v); break;
	}
}

// TODO: rename
function set_draw_fun(draw_ip) {
	if (scr.draw_interval_id) {
		window.clearInterval(scr.draw_interval_id)
	}
	// TODO: rename
	function draw_fun() {
		if (draw_ip) {
			vm.ip = draw_ip
			run()
		}
		// TODO: mouse x,y,button
	}
	scr.draw_interval_id = window.setInterval(draw_fun, 1000.0/60) // TODO: freq
}

