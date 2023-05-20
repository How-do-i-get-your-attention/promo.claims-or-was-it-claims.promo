package com.how_do_i_get_your_attention.phoneandtablet

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.os.Bundle
import android.view.SurfaceHolder
import android.view.SurfaceView
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val rootView = window.decorView
        rootView.setBackgroundColor(Color.BLACK)

        val uiSurfaceView = UISurfaceView(this)
        setContentView(uiSurfaceView)
    }

    private class UISurfaceView(context: Context) : SurfaceView(context), SurfaceHolder.Callback {
        private var animationThread: AnimationThread? = null

        init {
            holder.addCallback(this)
        }

        override fun surfaceCreated(holder: SurfaceHolder) {
            animationThread = AnimationThread(holder)
            animationThread?.start()
        }

        override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
            // Handle surface changes, if needed
        }

        override fun surfaceDestroyed(holder: SurfaceHolder) {
            var retry = true
            animationThread?.setRunning(false)

            while (retry) {
                try {
                    animationThread?.join()
                    retry = false
                } catch (e: InterruptedException) {
                    // Handle interruption
                }
            }
        }

        private inner class AnimationThread(private val surfaceHolder: SurfaceHolder) : Thread() {
            private var isRunning = false

            override fun run() {
                isRunning = true

                while (isRunning) {
                    val canvas: Canvas? = surfaceHolder.lockCanvas()

                    if (canvas != null) {
                        // Perform animation drawing operations on the canvas
                        drawAnimation(canvas)

                        surfaceHolder.unlockCanvasAndPost(canvas)
                    }

                    // Adjust the animation timing as needed
                    sleep(16) // 60 FPS (approximately)
                }
            }

            fun setRunning(isRunning: Boolean) {
                this.isRunning = isRunning
            }


        }
    }
}
