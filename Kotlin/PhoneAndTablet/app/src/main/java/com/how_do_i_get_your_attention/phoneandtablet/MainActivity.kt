package com.how_do_i_get_your_attention.phoneandtablet

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import android.view.View

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)


        val rootView = window.decorView

        rootView.setBackgroundColor(Color.BLACK)
        val circleView = CircleView(this)
        circleView.setBackgroundColor(Color.BLACK)
        setContentView(circleView)
    }

    private class CircleView(context: Context) : View(context) {
        private val paint = Paint()

        override fun onDraw(canvas: Canvas) {
            super.onDraw(canvas)

            // Set the color of the circle
            paint.color = Color.RED

            // Calculate the center coordinates and radius of the circle
            val centerX = width / 2f
            val centerY = height / 2f
            val radius = width / 4f

            // Draw the circle on the canvas
            canvas.drawCircle(centerX, centerY, radius, paint)
        }
    }
}
