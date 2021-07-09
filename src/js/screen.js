function init_screen(width=800, height=400) {	
	var screen = document.getElementById("screen")
	screen.innerHTML = `<canvas id="main_canvas" width="${width}" height="${height}""></canvas>`
	
	out = document.getElementById("output")
	cnv = document.getElementById("main_canvas")
	
	ctx = cnv.getContext("2d")
	ctx.imageSmoothingEnabled = false
	
	get_frame_buffer()
}

function fullscreen() {
	if (cnv.requestFullscreen) {
		cnv.requestFullscreen()
	} else if (cnv.mozRequestFullScreen) { /* Firefox */
		cnv.mozRequestFullScreen()
	} else if (cnv.webkitRequestFullscreen) { /* Chrome, Safari and Opera */
		cnv.webkitRequestFullscreen()
	} else if (cnv.msRequestFullscreen) { /* IE/Edge */
		cnv.msRequestFullscreen()
	}
}

// ---[ frame buffer based ]---------------------------------------------------

function get_frame_buffer() {
    image_data = ctx.getImageData(0, 0, ctx.canvas.width, ctx.canvas.height)
    frame_buffer = image_data.data
}

function pseti(i,r,g,b,a=255) {
	frame_buffer[4*i+0] = r
	frame_buffer[4*i+1] = g
	frame_buffer[4*i+2] = b
	frame_buffer[4*i+3] = a
}

function cls(r,g,b,a=255) {
	i_max = ctx.canvas.width * ctx.canvas.height
	for (i=0; i<i_max; i++) {
		pseti(i,r,g,b,a)
	}
}

function pset(x,y,r,g,b,a=255) {
	i = y*ctx.canvas.width+x
	pseti(i,r,g,b,a)
}

function flip() {
	ctx.putImageData(image_data, 0, 0)
}

// ----------------------------------------------------------------------------

init_screen() // XXX
