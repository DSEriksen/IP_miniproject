package imageprocessing.interpolation;

import org.eclipse.swt.graphics.ImageData;

/**
 * Created by Claudio on 18.05.2017.
 */
public class InterpolatorNearestNeighbour implements IInterpolator {
    @Override
    public int interpolate(ImageData data, double x, double y) {
        int u = (int) Math.round(x);
        int v = (int) Math.round(y);
        if (u >= 0 && u < data.width && v >= 0 && v < data.height) {
            return data.getPixel(u, v);
        } else {
            return -1;
        }
    }
}
