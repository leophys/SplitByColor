import cv2
import cv
import numpy as np

def get_circles(img, args):
    # TODO: for param2 in range(args.max_param2, args.min_param2, -1)
    circles = cv2.HoughCircles(img,cv.CV_HOUGH_GRADIENT,1,
                               args.min_dist,
                               param2=args.param2,
                               minRadius=args.min_radius,maxRadius=args.max_radius)
    if circles is None:
        raise Exception('error looking for circles')

    return circles


def highlight(img, circles, args):
    for i in circles[0,:]:
        # draw the outer circle
        cv2.circle(img,(i[0],i[1]),i[2],(255,255,0),2)
        # draw the center of the circle
        cv2.circle(img,(i[0],i[1]),2,(255,0,255),3)
    return img

def main(args):
    img = cv2.imread(args.image)
    cimg = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    if False:
        cv2.imshow('hsv', cv2.cvtColor(cimg, cv2.COLOR_HSV2BGR))
        cv2.waitKey(0)
        cv2.destroyAllWindows()

    lower = np.array([args.hue - args.hue_radius, args.min_saturation, args.min_value])
    upper = np.array([args.hue + args.hue_radius,255,args.max_value])
    lower = np.array(lower, dtype = "uint8")
    upper = np.array(upper, dtype = "uint8")
    # find the colors within the specified boundaries and apply
    # the mask
    mask = cv2.inRange(cimg, lower, upper)
    cimg = cv2.bitwise_and(cimg, cimg, mask = mask)
    if True:
        cv2.imshow('threshold', cv2.cvtColor(cimg, cv2.COLOR_HSV2BGR))
        cv2.waitKey(0)
        cv2.destroyAllWindows()
    gray = cv2.cvtColor(cv2.cvtColor(cimg, cv2.COLOR_HSV2BGR),
                        cv2.COLOR_BGR2GRAY)
    cv2.destroyAllWindows()
    circles = get_circles(gray, args)
    if args.coordinates:
        with open(args.coordinates, 'w') as buf:
            for c in circles[0]:
                p1, p2, r = c
                buf.write('%d\t%d\n' % (p1, p2))

    if args.show_result:
        highlight(img, circles, args)
        cv2.imshow('detected circles',img)
        cv2.waitKey(0)
        cv2.destroyAllWindows()

def get_parser():
    import argparse
    p = argparse.ArgumentParser()
    p.add_argument('image')

    col = p.add_argument_group('color')
    col.add_argument('--hue', default=59, type=int)
    col.add_argument('--hue-radius', default=10, type=int)
    col.add_argument('--min-saturation', default=150, type=int)
    col.add_argument('--min-value', default=150, type=int)
    col.add_argument('--max-value', default=255, type=int)

    det = p.add_argument_group('circle detection')
    det.add_argument('--param2', default=20, type=int, help='If it is lower, it will have more false positives')
    det.add_argument('--min-dist', default=100, type=int, help='Minimum distance between circles')
    det.add_argument('--min-radius', default=0, type=int)
    det.add_argument('--max-radius', default=0, type=int)

    p.add_argument('--coordinates', metavar='FILE', help='Write found coordinates to FILE')
    p.add_argument('--show-result', action='store_true', default=False, help='Show image with found circles')

    return p

if __name__ == '__main__':
    main(get_parser().parse_args())
