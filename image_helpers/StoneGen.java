package image_helpers;
import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class StoneGen {
    public static void main(String[] args) {
        generate();
    }

    static void generate() {
        BufferedImage image = new BufferedImage(32, 32, BufferedImage.TYPE_INT_RGB);
        
        Color[] colors = {new Color(127, 127, 127), new Color(107, 107, 107), new Color(97, 97, 97), new Color(89 ,89, 89)};
        
        // generate the image
        for (int i = 0; i < image.getWidth(); ++i) {
            for (int j = 0; j < image.getHeight(); ++j) {
                image.setRGB(i, j, colors[(int)(Math.random() * colors.length)].getRGB());
            }
        }

        save(image);
        return;
        
    }
    
    static void save(BufferedImage image) {
        try {
            File outputFile = new File("stone.png");
            ImageIO.write(image, "png", outputFile);
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

}