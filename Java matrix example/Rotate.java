package imageprocessing.translations;

import imageprocessing.IImageProcessor;
import imageprocessing.interpolation.IInterpolator;
import imageprocessing.interpolation.InterpolatorBilinear;
import imageprocessing.interpolation.InterpolatorIdeal;
import imageprocessing.interpolation.Interpolators;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.graphics.ImageData;
import utils.Matrix;
import utils.Parallel;

import javax.swing.*;

public class Rotate implements IImageProcessor {

    @Override
    public boolean isEnabled(int imageType) {
        return true;
    }

    @Override
    public Image run(Image input, int imageType) {
        ImageData inData = input.getImageData();
        String s = JOptionPane.showInputDialog("Rotationswinkel");
        if (s == null) {
            return null;
        } else {
            try {
                double rot = Double.parseDouble(s) * Math.PI / 180;
                IInterpolator i = Interpolators.chooseInterpolator();
                if (i == null) return null;
                int nwidth = (int) Math.round(inData.width * Math.cos(rot) + inData.height * Math.sin(rot));
                int nheight = (int) Math.round(inData.height * Math.cos(rot) + inData.width * Math.sin(rot));
                ImageData outData = new ImageData(nwidth, nheight, inData.depth, inData.palette);

                Matrix M = transform(inData.width / 2., inData.height / 2.,
                        (outData.width - inData.width) / 2., (outData.height - inData.height) / 2.,  rot);

                Parallel.For(0, outData.height, v -> {
                    for (int u = 0; u < outData.width; u++) {
                        double[] r = M.multiply(new double[]{u, v, 1});

                        int pixel = i.interpolate(inData, r[0], r[1]);
                        outData.setPixel(u, v, pixel > -1 ? pixel : 0);
                    }
                });
                return new Image(input.getDevice(), outData);
            } catch (NumberFormatException e) {
                return null;
            }
        }
    }

    private Matrix transform(double rx, double ry, double a, double b, double phi) {
        return Matrix.translation(rx, ry)
                .multiply(Matrix.rotation(-phi))
                .multiply(Matrix.translation(-(rx + a), -(ry + b)));
    }
}
